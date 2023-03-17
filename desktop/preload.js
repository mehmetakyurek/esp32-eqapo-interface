const { contextBridge, ipcRenderer } = require('electron')



contextBridge.exposeInMainWorld('events', {
    getFreqs: () => ipcRenderer.invoke('freq'),
    onFrequencyData: (callback) => ipcRenderer.on('frequencyData', callback)
})