
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

  cout << "Hi again!" << endl;

  int x = EM_ASM_INT({
    console.log('I received: ' + $0);
    return $0 + 1;
  }, 100);
  printf("%d\n", x);

  int y = EM_ASM_INT(
    let number = prompt("Please enter number: ");
    return number;);

  printf("%d\n", y);

  int z = EM_ASM_INT(
    var numeric;

    let letter = prompt("Please enter letter: ");
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

  return 0;
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


// python3 -m http.server

// http://0.0.0.0:8000/userInput/userInput.html

// emcc userInput.cpp -o userInput.html -sEXPORTED_FUNCTIONS=_main

