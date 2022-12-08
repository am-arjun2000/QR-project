var pn ="....oru south indian fruits kada...."
var vpa ="arjunb22@fbl";

var straw = 160;
var apple = 200;
var grapes = 170;
var pome = 220;
var gu = 60;
var total = 0;
var check = 1;
var gw;
var sfruit;
var perkg;
var adder = document.querySelector("#getme");
adder.addEventListener("click", getMe);
var by = document.querySelector("#out");
by.addEventListener("click", checkOut);
var cme = document.querySelector("#clr");
cme.addEventListener("click", clearMe);

function getMe() {
  sfruit = document.querySelector("#sf").value;
  perkg = parseFloat(document.querySelector("#entrybox").value);
  gw = document.querySelector("#writeme");
  if (Number.isNaN(perkg) === true) {
    alert("Only Numbers !!! Or The Field is Empty");
    check = 0;
  }
  else if (perkg<0) {
    alert("Negative value!! @stupid");
    check = 0;
  }
  else{
    check = 1;
  }
  if (check == 1) {
 
    switch (sfruit) {
      case "Strawberry":
        var st = straw * perkg;
        gw.value += "Strawberry " + perkg + " kg = " + st + " rs\r";
        total += st;
        perkg.value = "";
        break;
      case "Apple":
        var at = apple * perkg;
        gw.value += "Apple " + perkg + " kg = " + at + " rs\r";
        total += at;
        perkg.value = "";
        break;
      case "Grapes":
        var gr = grapes * perkg;
        gw.value += "Grapes " + perkg + " Kg  = " + gr + " rs\r";
        total += gr;
        perkg.value = "";
        break;
      case "Guava":
        var ga = gu * perkg;
        gw.value += "Guava " + perkg + " Kg  = " + ga + " RS\r";
        total += ga;
        perkg.value = "";
        break;
      case "Pomegranate":
        var pomy = pome * perkg;
        gw.value += "Pomegranate " + perkg + " Kg  = " + pomy + " rs\r";
        total += pomy;
        perkg.value = "";
        break;
    }

  }
}

function checkOut() {
  gw.value += "--------------------------------------------------------------\r";
  gw.value += "Your Total Bill Is = " + total + " :)\r";
  gw.value += "Thanks For Shopping! :)\r";
  gw.value += "--------------------------------------------------------------\r";

QRgen();  

  total = 0;
}

function QRgen(){
    var qrcode = new QRCode(document.getElementById("qrcode"), {
      text: "upi://pay?pn="+pn+"&pa="+vpa+"&cu=INR&mode=01&am="+total+"&orgid=000000&purpose=00",
      width: 128,
      height: 128,
    //   position: static,
    //   colorDark : "#5868bf",
    //   colorLight : "#ffffff",
    //   correctLevel : QRCode.CorrectLevel.H
  });
    }
function clearMe () {
  gw.value = "";

}


