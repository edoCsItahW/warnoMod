const ndf = require('./ndfNodeAPI.node')
const fs = require('fs')
const path = require('path')

fs.readFile("./node/test.ndf", (err, data) => {
    if (err) throw err;
    const ndfData = ndf.parse(data.toString())
    for (stmt of ndfData.statements)
        console.log(stmt.toString())
})

