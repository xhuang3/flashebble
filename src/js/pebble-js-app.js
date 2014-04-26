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
var deck_Num = 0;

var SAT_Vocabs = [
];


var GRE_Vocabs = [
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

function login_user(){
    var req = new XMLHttpRequest();
    req.open("POST", url + "users/login");
    req.send("username=test&password=123456");
    req.onreadystatechange = function () {
        console.log(req.readyState);
        if(req.readyState == 4){
            login_obj = JSON.parse(req.responseText);
            id = login_obj.id;
            //get_decks();
            get_cards();
        }   
    }
    return req.responseText;
}


function get_cards() {
    console.log("start getting cards");
    var req = new XMLHttpRequest();
    req.open("GET", url + "cards", true);
    var cookieString = "sid=" + id;
    console.log(cookieString);
    req.setRequestHeader('Cookie', cookieString);
    req.onreadystatechange = function () {
        console.log("get_cards: ready state "+req.readyState);
        if(req.readyState == 4){
            login_obj = JSON.parse(req.responseText);
            console.log(req.responseText);
            for(var i = 0 ; i < login_obj.length; i++){
                if(login_obj[i].deckname == "SAT") SAT_Vocabs.push([login_obj[i].title, login_obj[i].content]);
                if(login_obj[i].deckname == "GRE") GRE_Vocabs.push([login_obj[i].title, login_obj[i].content]);
            }
        }   
    }
    req.send();
}

function get_decks() {
    console.log("start getting decks");
    var req = new XMLHttpRequest();
    req.open("GET", url + "decks", true);
    var cookieString = "sid=" + id;
    console.log(cookieString);
    req.setRequestHeader('Cookie', cookieString);
    req.onreadystatechange = function () {
        if(req.readyState == 4){
            login_obj = JSON.parse(req.responseText);
            console.log(req.responseText);
        }   
    }
    req.send();
}

function cloud_to_pebble(id) {
    var title;
    var content;
    if(deck_Num == 0){
        console.log("SAT DICT");
        title = SAT_Vocabs[id][0];
        content = SAT_Vocabs[id][1];
    }else if(deck_Num == 1){
        console.log("GRE DICT");
        title = GRE_Vocabs[id][0];
        content = GRE_Vocabs[id][1];
    }
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
                            console.log("appmessage: id " + JSON.stringify(e.payload.ID));
                            console.log("deck id " + JSON.stringify(e.payload.DECK_ID));
                            if (e.payload){
                                deck_Num = e.payload.DECK_ID;
                                cloud_to_pebble(e.payload.ID);
                            }
                        }
);