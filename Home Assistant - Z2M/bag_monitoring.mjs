import * as exposes from 'zigbee-herdsman-converters/lib/exposes';

const e = exposes.presets;
const ea = exposes.access;

const CUSTOM_CLUSTER_ID = 65280; // 0xFF00
const ANALOG_INPUT_CLUSTER = 'genAnalogInput'; // Cluster 12 (0x000C)

export default {
    zigbeeModel: ['SACS-MONITOR'],
    model: 'SACS-MONITOR',
    vendor: 'Custom',
    description: 'Module de comptage de sacs à pelé',
    fromZigbee: [
        {
            cluster: CUSTOM_CLUSTER_ID,
            type: ['attributeReport', 'readResponse', 'commandReceive', 'raw'],
            convert: (model, msg, publish, options, meta) => {
                const result = {};
                const data = msg.data || {};
                
                if (data['0x0000'] !== undefined) result.bag_height = data['0x0000'];
                else if (data[0] !== undefined) result.bag_height = data[0];

                if (data['0x0001'] !== undefined) result.sensor_position = data['0x0001'];
                else if (data[1] !== undefined) result.sensor_position = data[1];

                return result;
            },
        },
        {
            cluster: ANALOG_INPUT_CLUSTER,
            type: ['attributeReport', 'readResponse'],
            convert: (model, msg, publish, options, meta) => {
                const result = {};
                if (msg.data['presentValue'] !== undefined) {
                    result.bag_count = msg.data['presentValue'];
                }
                return result;
            },
        },
    ],
    toZigbee: [
        {
            key: ['bag_height', 'sensor_position', 'bag_count'],
            convertSet: async (entity, key, value, meta) => {
                if (key === 'bag_count') {
                    const payload = { presentValue: value };
                    await entity.write(ANALOG_INPUT_CLUSTER, payload);
                    return { state: { [key]: value } };
                } else {
                    let attrId = 0x0000;
                    if (key === 'sensor_position') attrId = 0x0001;

                    const payload = {
                        [attrId]: {
                            value: value,
                            type: 0x29 // int16
                        }
                    };
                    await entity.write(CUSTOM_CLUSTER_ID, payload, { manufCode: 0x0000 });
                    return { state: { [key]: value } };
                }
            },
            convertGet: async (entity, key, meta) => {
                if (key === 'bag_count') {
                    await entity.read(ANALOG_INPUT_CLUSTER, ['presentValue']);
                } else {
                    let attrId = 0x0000;
                    if (key === 'sensor_position') attrId = 0x0001;

                    await entity.read(CUSTOM_CLUSTER_ID, [attrId], { manufCode: 0x0000 });
                }
            },
        },
    ],
    exposes: [
        e.numeric('bag_count', ea.STATE_GET)
            .withDescription('Nombre de sacs mesurés'),
        e.numeric('bag_height', ea.SET)
            .withUnit('cm')
            .withDescription('Hauteur totale du sac'),
        e.numeric('sensor_position', ea.SET)
            .withUnit('cm')
            .withDescription('Position des capteurs'),
    ],
};