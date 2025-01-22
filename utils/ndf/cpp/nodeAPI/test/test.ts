import { parse, ObjectDef } from 'ndf-node-api';
import * as fs from 'fs';


//fs.readFile("test/test.ndf", (err, data) => {
//    if (err) throw err;
//    const program = parse(data.toString());
//    for (const stmt of program.statements)
//        console.log(stmt.pos())
//})
const program = parse("Descriptor_Deck_Division_Integrate_mutil is TDeckDivisionDescriptor (\n" +
    "    a = 1\n" +
    ")");

console.log(program.toString())
