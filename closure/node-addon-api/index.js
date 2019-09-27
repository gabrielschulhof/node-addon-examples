const closure = require('bindings')('closure');
const outerObject = {};
const functionWithClosure = closure.createClosure(outerObject);
functionWithClosure();
console.log(JSON.stringify(outerObject, null, 4));
