/**
 * @file test.ts
 * @author edocsitahw
 * @version 1.1
 * @date 2024/12/20 上午10:34
 * @desc
 * */

import { TokenProcessor, Lexer } from './index'
import * as fs from 'fs';

fs.readFile('test.ndf', 'utf8', (err, data) => {
  if (err) {
    console.error(err);
    return;
  }
  const tokens = TokenProcessor.process(Lexer.tokenize(data, true));
});
