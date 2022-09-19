
import { Base64 } from 'js-base64';

function getEncode(str) {
    return Base64.encode(str);
}


function getDecode(str){
    let res;
    try{
        res = Base64.decode(str);
    } catch(err) {
        console.log(err);
        res = "解码错误";
    }
    return res
}

export {getEncode, getDecode}