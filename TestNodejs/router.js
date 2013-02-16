function route(handle, pathname){
    console.log("About to route a request");

    if( typeof handle[pathname] == 'funciton'){
        handle[pathname]();
    }
    else {
        console.log("No request handle found for" + pathname );
    }
}

exports.route = route;
