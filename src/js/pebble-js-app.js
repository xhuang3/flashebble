function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}

function cloud_to_pebble() {
    var title = "Hello";
    var content = "I am Andy";
    var dict = {"TITLE_KEY":title, "CONTENT_KEY":content};
    Pebble.sendAppMessage(dict);
}

function cloud_to_pebble2() {
    var title = "Byebye";
    var content = "I am Simba";
    var dict = {"TITLE_KEY":title, "CONTENT_KEY":content};
    Pebble.sendAppMessage(dict);
}

// Set callback for the app ready event
Pebble.addEventListener("ready",
                        function(e) {
                          cloud_to_pebble();
                        });

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
                        function(e) {
                          cloud_to_pebble2();
                        }
);