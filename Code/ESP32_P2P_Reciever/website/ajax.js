setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getState();
}, 2000); //2000mSeconds update rate


function setAllButtonsLow() {
    var buttons = document.getElementsByClassName("button");
    for(var i = 0, l = buttons.length; i < l; i++) {
        var button = buttons[i];
        button.style.backgroundColor="#34495e";
      }
}


function setButtonHigh(id) {
    document.getElementById(id).style.backgroundColor="#3498db";
}


function setState(state) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", state, true);
    xhttp.send();
}


function getState() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText == "bell") {
                alert("Somebody is calling!");
            }
            else {
                setAllButtonsLow();
                setButtonHigh(this.responseText);
            }
        }
    };
    xhttp.open("GET", "getState", true);
    xhttp.send();
}

function getBell() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById(this.responseText).style.backgroundColor="red";
        }
    };
    xhttp.open("GET", "getBell", true);
    xhttp.send();
}