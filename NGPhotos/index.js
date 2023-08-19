const { app, BrowserWindow } = require('electron')

const createWindow = () => {
    // windows in full screen
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            preload: __dirname + '/preload.js'
        },
        fullscreen: false,
    })
    
    win.loadFile('index.html')
}

app.whenReady().then(() => {    
    createWindow()
})

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit()
    }
}
)

