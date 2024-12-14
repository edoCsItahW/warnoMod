const ndf = require('./ndfNodeAPI.node')
const fs = require('fs')
const path = require('path')

fs.readFile("E:\\codeSpace\\codeSet\\ndf\\warnoMod\\GameData\\Generated\\Gameplay\\Decks\\Divisions.ndf", (err, data) => {
    if (err) throw err;
    const ndfData = ndf.parse(data.toString(), true)
    console.log(ndfData)
})

