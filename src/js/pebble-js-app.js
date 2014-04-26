function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}
// Set callback for the app ready event
Pebble.addEventListener("ready",
                        function(e) {
                          var title = "Hello";
                          var content = "I am Andy";
                          var dict = {"TITLE_KEY":title, "CONTENT_KEY":content};
                          Pebble.sendAppMessage(dict);
                        });

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
                        function(e) {
                          console.log("Received message: " + e.payload.title);});