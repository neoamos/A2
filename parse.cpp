/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

#include <iostream>
#include <cstdlib>
<<<<<<< HEAD
#include <string>
=======
#include <string>    
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
using namespace std;

#include "scan.h"


const string names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                        "eql", "neql", "less", "more", "leql", "meql",
                        "if", "fi", "do", "od", "check"};

static token input_token;

<<<<<<< HEAD
string token_string(token t){
  string s = "\"" + names[t] + "\"";
  if(t == t_id || t == t_literal){
    s = s + " (" + token_image + ")";
  }
  return s;
}
=======
bool assignId = false;
string arg1;
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c

void error (token unexpected_token) {
    cout << "Syntax error: unexpected token " +token_string(unexpected_token) + " on line ";
    cout << ln;
    cout << ", col ";
    cout << col << endl;
    string e = "Syntax error: unexpected token " + token_string(unexpected_token);
    throw e;
    //exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
<<<<<<< HEAD
        if (input_token == t_id  || input_token == t_literal)
            //printf (": %s", token_image);
            cout << ": " << token_image;
        //printf ("\n");
=======
        if (input_token == t_id || input_token == t_literal)
            cout << token_image ;
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
        cout << endl;
        input_token = scan ();
    }else{
      cout << "Syntax error: received an " +token_string(input_token) + " when expecting an " + token_string(expected) + " on line ";
      cout << ln;
      cout << ", col ";
      cout << col << endl;
      throw "Syntax error: received an "+ token_string(input_token) +" when expecting an " + token_string(expected);
    }
}

void program ();
void stmt_list ();
void stmt ();
void expr ();
void term_tail ();
void term ();
void factor_tail ();
void factor ();
void add_op ();
void mul_op ();

void rln ();
void rln_op ();
void expr_tail ();

void program () {
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_if:
            case t_do:
            case t_check:
            case t_eof:
                //cout << "predict program --> stmt_list eof" << endl;
                cout << "(program" << endl;
                cout << "[ ";
                stmt_list ();
                match (t_eof);
                cout << "\n]" << endl;
                cout << "\n)" << endl;
                break;
            default: error (input_token);
        }
    }catch(const string e){
<<<<<<< HEAD
      while(1){
        if(input_token==t_id ||
          input_token==t_read ||
          input_token==t_write ||
          input_token==t_if ||
          input_token==t_do ||
          input_token==t_check){
          program();
          return;
        }else if(
          input_token==t_eof
          ){
          return;
        }else{
          input_token = scan();
        }
      }
=======
        cout << e << endl;
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
    }
}

void stmt_list () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            //cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt ();
            stmt_list ();
            break;
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict stmt_list --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void stmt () {
  try{
    switch (input_token) {
        case t_id:
            //cout << "predict stmt --> id gets rln" << endl;
            cout << "(:= ";
            assignId = true;
            match (t_id);
            match (t_gets);
            rln ();
            cout << ")" << endl; 
            break;
        case t_read:
            //cout <<  "predict stmt --> read id" << endl;
<<<<<<< HEAD
=======
            cout << "(read ";
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
            match (t_read);
            match (t_id);
            cout << ")" << endl;
            break;
        case t_write:
<<<<<<< HEAD
            cout << "predict stmt --> write rln" << endl;
=======
            //cout <<  "predict stmt --> write rln" << endl;
            cout << "(write ";
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
            match (t_write);
            rln ();
            cout << ")" << endl;
            break;
        case t_if:
            //cout << "predict stmt --> if rln stmt_list fi" <<endl;
            cout << "(if " << endl;
            match (t_if);
            cout << "(";
            rln ();
            cout << ")";
            cout << "\n[ ";
            stmt_list ();
            cout << "\n]" << endl;
            match (t_fi);
            cout << ")" << endl;
            break;
        case t_do:
            //cout << "predict stmt --> do stmt_list od" <<endl;
            cout << "(do " << endl;
            match (t_do);
            cout << "[ ";
            stmt_list ();
            cout << "\n]" << endl;
            match (t_od);
            cout << ")" << endl;
            break;
        case t_check:
            //cout << "predict stmt --> check rln" <<endl;
            cout << "(check ";
            match (t_check);
            rln ();
            cout << ")" << endl;
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
<<<<<<< HEAD
      if(input_token==t_id ||
=======
      if(input_token==t_eof ||
        input_token==t_id ||
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check){
        stmt();
        return;
      }else if(
        input_token==t_eof ||
        input_token==t_od ||
        input_token==t_fi
        ){
        return;
      }else{
        input_token = scan();
      }
    }
  }
}

void expr () {
  try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            //cout << "predict expr --> term term_tail" << endl;
            term ();
            term_tail ();
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
<<<<<<< HEAD
      if(input_token==t_lparen  ||
=======
      if(input_token==t_lparen ||
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
        input_token==t_id ||
        input_token==t_literal){
          expr();
          return;
      }else if(
        input_token==t_rparen ||
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check ||
        input_token==t_fi ||
        input_token==t_od ||
        input_token==t_eof ||
        input_token==t_eql ||
        input_token==t_neql ||
        input_token==t_less ||
        input_token==t_more ||
        input_token==t_leql ||
        input_token==t_meql
      ){
        return;
      }else{
        input_token = scan();
      }
    }
  }
}

void expr_tail () {
    switch (input_token) {
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:
            //cout << "predict expr_tail --> rln_op expr" << endl;
            rln_op ();
            expr ();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict expr_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }

}

void rln () {
  try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            //cout << "predict expr --> expr expr_tail" << endl;
            expr ();
            expr_tail ();
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_lparen ||
        input_token==t_id ||
        input_token==t_literal){
          rln();
          return;
      }else if(
        input_token==t_rparen ||
<<<<<<< HEAD
        input_token==t_read  ||
        input_token==t_write  ||
        input_token==t_if  ||
        input_token==t_do  ||
        input_token==t_check  ||
        input_token==t_fi  ||
        input_token==t_od  ||
=======
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check ||
        input_token==t_fi ||
        input_token==t_od ||
>>>>>>> ba0a1245fac8b9826cbee406c0c5d453992c977c
        input_token==t_eof
      ){
        return;
      }else{
        input_token = scan();
      }
    }
  }
}

void term_tail () {
    switch (input_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail" << endl;
            add_op ();
            term ();
            term_tail ();
            break;
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            cout << "predict term_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail" << endl;
            factor ();
            factor_tail ();
            break;
        default: error (input_token);
    }
}

void factor_tail () {
    switch (input_token) {
        case t_mul:
        case t_div:
            //cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict factor_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void factor () {
    switch (input_token) {
        case t_id :
            //cout << "predict factor --> id"  << endl;
            if (!assignId) {cout << "(id ";}
            match (t_id);
            if (!assignId) {cout << ")";}
            assignedId = false;
            break;
        case t_literal:
            //cout << "predict factor --> literal"  << endl;
            cout << "(num ";
            match (t_literal);
            cout << ")";
            break;
        case t_lparen:
            //cout << "predict factor --> lparen expr rparen"  << endl;
            match (t_lparen);
            rln ();
            match (t_rparen);
            break;
        default: error (input_token);
    }
}

void add_op () {
    switch (input_token) {
        case t_add:
            cout << "predict add_op --> add" << endl;
            match (t_add);
            break;
        case t_sub:
            cout << "predict add_op --> sub" << endl;
            match (t_sub);
            break;
        default: error (input_token);
    }
}

void mul_op () {
    switch (input_token) {
        case t_mul:
            cout << "predict mul_op --> mul" << endl;
            match (t_mul);
            break;
        case t_div:
            cout << "predict mul_op --> div\n" << endl;
            match (t_div);
            break;
        default: error (input_token);
    }
}

void rln_op () {
    switch (input_token) {
        case t_eql:
            cout << "predict rln_op --> eql" << endl;
            match (t_eql);
            break;
        case t_neql:
            cout << "predict rln_op --> neql" << endl;
            match (t_neql);
            break;
        case t_less:
            cout << "predict rln_op --> t_less" << endl;
            match (t_less);
            break;
        case t_more:
            cout << "predict rln_op --> t_more" <<endl;
            match (t_more);
            break;
        case t_leql:
            cout << "predict rln_op --> leql" << endl;
            match (t_leql);
            break;
        case t_meql:
            cout << "predict rln_op --> meql" << endl;
            match (t_meql);
            break;
        default: error (input_token);
    }
}

int main () {
    input_token = scan ();
    program ();
    return 0;
}
