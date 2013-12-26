#ifndef EDITM_H
#define EDITM_H

#include "child.h"

class PlainTextEdit;
class Form;
class Pane;

// ---------------------------------------------------------------------
class Editm : public Child
{
  Q_OBJECT

public:
  Editm(string n, string s, Form *f, Pane *p);
  void cmd(string p, string v);
  void set(string p, string v);
  string state();

private slots:

private:
  void setselect(PlainTextEdit *w, int bgn, int end);

};

#endif
