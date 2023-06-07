const express = require('express');
const cors = require('cors');
const app = express();


app.listen(3000,()=>{
    console.log("servidor rodando na porta 3000, para acessar use http://localhost:3000")
})