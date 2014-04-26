/*
 *1 Abhor: hate
 *2 Bigot: narrow-minded
 *3 Counterfeit: fake; false
 *4 Enfranchise: give voting rights
 *5 Hamper: hinder; obstruct
 *6 Kindle: to start a fire
 *7 Noxious: harmful; poisonous; lethal
 *8 Placid: calm; peaceful
 *9 Remuneration: payment for work done
 *10 Talisman: lucky charm
 *
 *11 Abrasive: wough; coarse; harsh
 *12 Bilk: cheat; defraud
 *13 Convert: hidden; undercover
 *14 Engender: cause
 *15 Hangar: storage area for a plane
 *16 Knotty: complex; difficult to solve
 *17 Nuance: something subtle; a fine shade of meaning
 *18 Plagiarism: taking credit for someone else's writing or ideas
 *19 Renown: fame
 *20 Tangent: Tangent
 *
 *
 */
var vocabs = [
             ["Abhor", "hate"],
             ["Bigot", "narrow-minded"],
             ["Counterfeit", "fake; false"],
             ["Enfranchise", "give voting rights"],
             ["Hamper", "hinder; obstruct"],
             ["Kindle", "to start a fire"],
             ["Noxious", "harmful; poisonous; lethal"],
             ["Placid", "calm; peaceful"],
             ["Remuneration", "payment for work done"],
             ["Talisman", "lucky charm"],
             
             ["Abrasive", "wough; coarse; harsh"],
             ["Bilk", "cheat; defraud"],
             ["Convert", "hidden; undercover"],
             ["Engender", "storage area for a plane"],
             ["Knotty", "complex; difficult to solve"],
             ["Nuance", "something subtle"],
             ["Plagiarism", "taking credit for someone's ideas"],
             ["Renown", "fame"],
             ["Tangent", "Tangent"]

];

function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}


function cloud_to_pebble(id) {
    var title = vocabs[id][0];
    var content = vocabs[id][1];
    id++;
    var dict = {"TITLE_KEY":title, "CONTENT_KEY":content};
    Pebble.sendAppMessage(dict);
}

// Set callback for the app ready event
Pebble.addEventListener("ready",
                        function(e) {
                        });

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
                        function(e) {
                            console.log("Appmessage received.");
                            console.log(JSON.stringify(e.payload.ID));
                            if (e.payload){
                                console.log(JSON.stringify(e.payload.ID));
                                cloud_to_pebble(e.payload.ID);
                            }
                        }
);