const express = require('express');
const cors = require('cors');
const app = express();

app.get('/',(req,res)=>{
    res.json({message: "Bem vindo a minha api, para a aula de IOT"})
})
app.listen(3000,()=>{
    console.log("servidor rodando na porta 3000, para acessar use http://localhost:3000")
})