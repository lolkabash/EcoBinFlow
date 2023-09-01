// very real persistent database, production ready

import { BinRecord } from "./shared";

export class Database {
    private binStorage: { [k: number]: BinRecord } = {};

    constructor() {}

    public addBin(id: number) {
        this.binStorage[id] = { id, fillLevel: 0, position: [0, 0] };
    }

    public updateBin(id: number, fillLevel: number) {
        if (id in this.binStorage) {
            Object.assign(this.binStorage[id], {
                fillLevel
            });
        } else {
            throw new Error(`bin id ${id} does not exist`);
        }
    }

    public get(): BinRecord[] {
        return Object.values(this.binStorage);
    }

}
