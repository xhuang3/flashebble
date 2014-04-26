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


var notes = [
    ["Page1", "This is a reminder for XXXX"]
]

var username = "test";
var password = "123456";
var login_obj;
var id;
var url = "http://fengpanhome.com:8080/";
var deckid = "17b94e854f64886d";
var userid = "ef118643c72b0b73";

function login_user(){
    var req = new XMLHttpRequest();
    req.open("POST", url + "users/login");
    req.send("username=test&password=123456");
    req.onreadystatechange = function () {
        console.log(req.readyState);
        if(req.readyState == 4){
            login_obj = JSON.parse(req.responseText);
            id = login_obj.id;
            console.log(id);
            console.log("getting cards");
            get_cards()
        }   
    }
    return req.responseText;
}


function get_cards() {
    console.log("start getting cards");
    var req = new XMLHttpRequest();
    req.open("POST", url + "decks", true);
    
    var cookie2 = JSON.stringify([
{
    "domain": "fengpanhome.com:8080",
    "hostOnly": true,
    "httpOnly": true,
    "name": "sid",
    "path": "decks",
    "secure": false,
    "session": true,
    "storeId": "0",
    "userid" : "ef118643c72b0b73",
    "value": "650e6bda7744716c129515a7c47a94a915fe070ef1fb3a1f8fef486f319ce93092c8b3cee0a2cfdf79c5a7a332552cabcba92d1172863598a871abe2632388f5",
    "id": 1
}
]);
    var cookie = "hostOnly=true&httpOnly=true&secure=false&session=true&name=sid&userid=ef118643c72b0b73&value=650e6bda7744716c129515a7c47a94a915fe070ef1fb3a1f8fef486f319ce93092c8b3cee0a2cfdf79c5a7a332552cabcba92d1172863598a871abe2632388f5"
    
    console.log(cookie);
    
    req.onreadystatechange = function () {
        console.log("get_cards: ready state "+req.readyState);
        if(req.readyState == 4){
            //var cards = JSON.parse(req.responseText);
            console.log(req.responseText);
            //console.log("Here: " + cards);
        }   
    }
    req.send();
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
                            console.log("Ready: start login\n");
                            login_user();
                            
                        });

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
                        function(e) {
                            console.log("appmessage: id" + JSON.stringify(e.payload.ID));
                            if (e.payload){
                                cloud_to_pebble(e.payload.ID);
                                
                            }
                        }
);