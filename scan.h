/* Definitions the scanner shares with the parser
    Michael L. Scott, 2008-2017.
*/

enum token {t_read, t_write, t_id, t_literal, t_gets,
                t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof, 
                t_eql, t_neql, t_less, t_more, t_leql, t_meql,
            	t_if, t_fi, t_do, t_od, t_check};

extern char token_image[100];

extern token scan();
