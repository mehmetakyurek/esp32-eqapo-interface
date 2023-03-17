const fs = require('fs');
const path = require('path');
const main = require('.');


const eqConfigPath = "C:\\Program Files\\EqualizerAPO\\config";
//
/**
 * @type fs.FileHandle
 */
let file;

function createString10() {
    let str = "GraphicEQ: ";
    let sum = 0;
    main.freqs.map((val, index) => {
        if (index % 10 === 0) {
            str += freqMap[index / 10] + " " + ((sum - 120) / 100) + ";";
            sum = 0;
        } else {
            sum += val;
        }
    })
    return str;

}
function map(x, in_min, in_max, out_min, out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

function createString() {
    let str = "GraphicEQ:";
    for (let i = 1; i <= 100; i++) {
        const freq =
            i <= 25 ? (map(i, 1, 25, 1, 20)) :
                i <= 50 ? (map(i, 26, 50, 20, 200)) :
                    i <= 75 ? (map(i, 51, 75, 200, 2000)) :
                        i <= 100 ? (map(i, 76, 100, 2000, 20000)) : 0;
        str += freq.toFixed(2) + " " + map(main.freqs[i - 1], 0, 240, -20, 20).toFixed(2) + (i === 100 ? "" : ";");
    }
    return str;
}

module.exports.onFreqChange = () => {
    writeTofile();
}

let timeout;
function writeTofile() {
    if (timeout) clearInterval(timeout);
    timeout = setTimeout(() => {

        let stream = fs.createWriteStream(path.join(eqConfigPath, 'espeq.txt'), {
            start: 0,
            encoding: 'utf-8',
            flags: "w+",
            autoClose: true
        });
        stream.write(createString(), () => {
            stream.close();
            stream = null;
        })
    }, 10)
}