setInterval(function() {
    // Call a function repetatively with 2 Second interval
    getBell();
}, 2000); //2000mSeconds update rate


function setAllButtonsLow() {
    var buttons = document.getElementsByClassName("button");
    for(var i = 0, l = buttons.length; i < l; i++) {
        var button = buttons[i];
        button.style.backgroundColor="#34495e";
      }
}


function setButtonHigh(id) {
    document.getElementById(id).style.backgroundColor="#3498db"
}


function setState(state) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", state, true);
    xhttp.send();
    setAllButtonsLow()
    setButtonHigh(state)
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