import axios from "axios";
import { BinRecord } from "../../src/shared";

const API_URL = "http://localhost:8080";

const server = axios.create({ baseURL: API_URL })

export class BinApi {
    public static getAll(): Promise<BinRecord[]> {
        return server.get("/bins").then(r => r.data)
    }
}