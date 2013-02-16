var exec = require("child_process").exec;

function start(response)
{
    console.log("Request handle 'start' was called");
    exec("ls -lah", function(error, stdout, stderr ){
        response.writeHead(200, {"Content-type":"text/plain"});
        response.write(stdout);
        response.end();
    }
        );
}


function update(response)
{
    console.log("Request handle 'update' was called");
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.write("Hello Update");
    response.end();
}


exports.start = start;
exports.update= update;
