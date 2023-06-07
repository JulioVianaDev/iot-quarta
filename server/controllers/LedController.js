const cron = require('cron');
let ledState = false;

const toggleLED =()=>{
    ledState = !ledState
    console.log(`Led state: ${ledState}`);
}

const job = new cron.CronJob('*/1 * * * * *',toggleLED)

job.start();

module.exports = class LedController{
    static async init(req,res){
        res.send({message: "Tudo funcionando normalmente"})
    }
    static async ledState(req,res){
        res.json({state: ledState})
    }
}