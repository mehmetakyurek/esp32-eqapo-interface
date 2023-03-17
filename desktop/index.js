let electron, { BrowserWindow, app, ipcMain } = require('electron')
const path = require('node:path');
let freqs = Array(100);
const { onFreqChange } = require('./file');
readData();
let udp = require('./udp');
let mainWindow;

app.whenReady().then(() => {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js')
        }
    })
    mainWindow.loadFile(path.join(__dirname, 'ui/index.html'));
    mainWindow.on('ready-to-show', () => {
    })
})

udp.socket.on('message', (msg, rinfo) => {
    const [freq, db] = msg.toString().split(',').map(e => Number(e));
    if (freq != NaN && db != NaN) {
        freqs[freq - 1] = 240 - db;
        mainWindow.webContents.send("frequencyData", freq - 1, map(freqs[freq - 1], 0, 240, -20, 20));
    }

    onFreqChange();
});
function map(x, in_min, in_max, out_min, out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
ipcMain.handle('freq', () => {
    const f = Array();
    for (let i = 1; i <= 100; i++) {

        f.push((i <= 25 ? (map(i, 1, 25, 1, 20)) :
            i <= 50 ? (map(i, 26, 50, 20, 200)) :
                i <= 75 ? (map(i, 51, 75, 200, 2000)) :
                    i <= 100 ? (map(i, 76, 100, 2000, 20000)) : 0))

    }
    console.log(f);
    return Promise.resolve(f);
})

function readData() {
    for (let i = 0; i < 100; i++) {
        freqs[i] = 100;
    }
}
module.exports.freqs = freqs;