import { fetch } from '@real/http'
const gree = require("./greet.js")
let res = fetch("httpbin.org/get")
console.log(res)
console.assert(res == '',() => {
    console.print("Response IS Not Empty")
    console.log("!")
})