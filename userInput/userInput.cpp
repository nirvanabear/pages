
#include <emscripten.h>
#include <iostream>

using namespace std;

int main() {
  char howdy[14] = "hello world!";
  EM_ASM(
    alert('hello world!');
    // alert(howdy);
    // throw 'all done';
  );


  int x = EM_ASM_INT({
    console.log('I received: ' + $0);
    return $0 + 1;
  }, 100);
  printf("%d\n", x);

  cout << "Input an integer:" << endl;

  // int y = -1;
  // while (y < 0) {

  string enterNumber = "Please enter number: ";
  int y = EM_ASM_INT(
    let number = prompt("Please enter number: ");
    return number;
    );

  printf("%d\n", y);

  

  int z = EM_ASM_INT(
    var numeric;
    let letter = prompt("Please enter y/n: ");
    if (letter == "n") {
      numeric = 0;
    }
    if (letter == "y") {
      numeric = 1;
    }
    return numeric;);

  printf("%d\n", z);

  char output;
  if (z == 0) {
    output = 'n'; 
  }
  if (z == 1) {
    output = 'y';
  }
  printf("%c\n", output);

  cout << "Let's pass a string to js!" << endl;



  int a = EM_ASM_INT({
    let placeholder = prompt("Hello " + $0);
    return 0;
  }, "World");
  printf("%d\n", a);




  string yesNo = "";


  cout << "Boolean time!" << endl;

  bool b = EM_ASM_INT({
    let boolbaby = confirm("Are you cool?");
    return boolbaby;
  }, "World");
  printf("%d\n", b);

  return 0;
} // End of main()

string convertYesNo(int jsOut) {
    if (jsOut == 0) {
        return "n";
    }
    else if (jsOut == 1) {
        return "y";
    }
    else
        return "0";
}



  // TODO // Restrict user input to 'y' or 'n' answers only.

  // while (numeric != "n" || numeric != "y") {
  //   let letter = prompt("Please enter letter: ");
  //   if (letter == "n") {
  //     numeric = 0;
  //   }
  //   else if (letter == "y") {
  //     numeric = 1;
  //   }
  // }

    // var number = -1;ß
    // (async () => {
    // await while (number < 0) {
    // let number = document.getElementById("myInput").value;
    // } 
    // return number;})
    // return number;
    // (async () => {
    // // let pause = 0;
    // // while (!pause) {
    //   await sleep(5000);
    //   let number = document.getElementById("myInput").value;
    //   // if (number.something) pause = 1;
    //   // }
    // });





// python3 -m http.server

// http://0.0.0.0:8000/userInput/userInput.html

// emcc userInput.cpp -o userInput.html -sEXPORTED_FUNCTIONS=_main

//emcc userInput.cpp -o userInput.html -sEXPORTED_FUNCTIONS=_main -O3 --shell-file html_template/shell_minimal.html
