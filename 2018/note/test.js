function list(names) {
    let str = "";
    if (name.length === 0) {
        return [];
    } else if (name.length === 1) {
        return [names.name];
    } else {
        let str = "";
        let arr = [];
        for (let i of names) {
            arr.push(i.name);
            str = arr.join(", ").replace(arr.lastIndexOf(","), " &");
        }
        return arr;
    }
}