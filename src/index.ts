import express from "express";
import bodyParser from "body-parser";
import { Database } from "./db";

const PORT = 8080;

const app = express();
const database = new Database();
database.addBin(1);

app.use((req, res, next) => {
    res.header("Access-Control-Allow-Origin", "http://localhost:5173");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.post("/bins/:id", bodyParser.text({type: '*/*'}), (req, res) => {
    try {
        const id = parseInt(req.params.id);
        const [fillLevel] = (<string>req.body).split(",");
        database.updateBin(id, parseInt(fillLevel));
        console.log(`updating bin ${id} to level: ${fillLevel}`);
        res.sendStatus(200);
    } catch (error) {
        console.error(error);
        res.sendStatus(400);
    }

});

// returns all bin data
app.get("/bins", (req, res) => {
    res.json(database.get());
});

app.listen(PORT, () => {
    console.log(`started server on port ${PORT}`);
});
