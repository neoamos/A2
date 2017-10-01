/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

#include "scan.h"


const string names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                        "eql", "neql", "less", "more", "leql", "meql",
                        "if", "fi", "do", "od", "check"};

static token input_token;

string token_string(token t){
  string s = "\"" + names[t] + "\"";
  if(t == t_id || t == t_literal){
    s = s + " (" + token_image + ")";
  }
  return s;
}

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
        //printf ("matched %s", names[input_token]);
        cout << "matched " << names[input_token];
        if (input_token == t_id  || input_token == t_literal)
            //printf (": %s", token_image);
            cout << ": " << token_image;
        //printf ("\n");
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
              //printf ("predict program --> stmt_list eof\n");
              cout << "predict program --> stmt_list eof" << endl;
              stmt_list ();
              match (t_eof);
              break;
          default: error (input_token);
      }
    }catch(const string e){
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
            cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt ();
            stmt_list ();
            break;
        case t_fi:
        case t_od:
        case t_eof:
            //printf ("predict stmt_list --> epsilon\n");
            cout << "predict stmt_list --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void stmt () {
  try{
    switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets rln" << endl;
            match (t_id);
            match (t_gets);
            rln ();
            break;
        case t_read:
            //cout <<  "predict stmt --> read id" << endl;
            match (t_read);
            match (t_id);
            break;
        case t_write:
            cout << "predict stmt --> write rln" << endl;
            match (t_write);
            rln ();
            break;
        case t_if:
            cout << "predict stmt --> if rln stmt_list fi" <<endl;
            match (t_if);
            rln ();
            stmt_list ();
            match (t_fi);
            break;
        case t_do:
            cout << "predict stmt --> do stmt_list od" <<endl;
            match (t_do);
            stmt_list ();
            match (t_od);
            break;
        case t_check:
            cout << "predict stmt --> check rln" <<endl;
            match (t_check);
            rln ();
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_id ||
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
            cout << "predict expr --> term term_tail" << endl;
            term ();
            term_tail ();
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_lparen  ||
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
            cout << "predict expr_tail --> rln_op expr" << endl;
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
            cout << "predict expr_tail --> epsilon" << endl;
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
            cout << "predict expr --> expr expr_tail" << endl;
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
        input_token==t_read  ||
        input_token==t_write  ||
        input_token==t_if  ||
        input_token==t_do  ||
        input_token==t_check  ||
        input_token==t_fi  ||
        input_token==t_od  ||
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
            cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
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
            cout << "predict factor_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void factor () {
    switch (input_token) {
        case t_id :
            cout << "predict factor --> id"  << endl;
            match (t_id);
            break;
        case t_literal:
            cout << "predict factor --> literal"  << endl;
            match (t_literal);
            break;
        case t_lparen:
            cout << "predict factor --> lparen expr rparen"  << endl;
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
