
#include <QBoxLayout>
#include <QButtonGroup>
#include <QMenuBar>
#include <QSignalMapper>

#include "wd.h"
#include "font.h"
#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "child.h"
#include "isigraph2.h"
#include "menus.h"
#include "opengl2.h"

extern int jedo(char*);

// ---------------------------------------------------------------------
Form::Form(string s, string p, string loc, QWidget *parent)
{
  Q_UNUSED(parent);
  id=s;
  child=0;
  evtchild=0;
  locale=loc;
  fontdef=0;
  menubar=0;
  closed=false;
  shown=false;
  setAttribute(Qt::WA_DeleteOnClose);
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
  closeok=m.contains("closeok");
  escclose=m.contains("escclose");
  setpn(s);
  layout=new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  addpane(0);
  signalMapper=new QSignalMapper(this);
  connect(signalMapper,SIGNAL(mapped(QWidget *)),
          this,SLOT(buttonClicked(QWidget *)));
}

// ---------------------------------------------------------------------
Form::~Form()
{
  Child *n;
  for (int i=0; i<children.size(); i++)
    if ("isigraph"==(n=children.at(i))->type) {
      if (((Isigraph2*)n->widget)->painter) {
        ((Isigraph2*)n->widget)->painter->end();
        delete ((Isigraph2*)n->widget)->painter;
        ((Isigraph2*)n->widget)->painter=0;
      }
    } else if ("opengl"==(n=children.at(i))->type) {
      if (((Opengl2*)n->widget)->painter) {
        ((Opengl2*)n->widget)->painter->end();
        delete ((Opengl2*)n->widget)->painter;
        ((Opengl2*)n->widget)->painter=0;
      }
    }
  if (this==form) form = 0;
  if (this==evtform) evtform = 0;
  Forms.removeOne(this);
}

// ---------------------------------------------------------------------
void Form::addchild(Child *c)
{
  child=c;
  children.append(c);
}

// ---------------------------------------------------------------------
void Form::addmenu()
{
  menubar= new Menus("menu","",this,0);
  addchild((Child *) menubar);
  layout->insertWidget(0,child->widget);
}

// ---------------------------------------------------------------------
Pane *Form::addpane(int n)
{
  pane=new Pane(n,this);
  panes.append(pane);
  return pane;
}

// ---------------------------------------------------------------------
void Form::buttonClicked(QWidget *w)
{
  Child *child=(Child *) w;
  child->event="button";
  signalevent(child);
}

// ---------------------------------------------------------------------
void Form::closeEvent(QCloseEvent *e)
{
  if (closeok || closed) {
    e->accept();
    return;
  }
  e->ignore();
  event="close";
  form=this;
  signalevent(0);
  if (closed) {
    e->accept();
  } else e->ignore();
}

// ---------------------------------------------------------------------
// close if not the main pane
void Form::closepane()
{
  if (panes.size()<=1) return;
  panes.removeLast();
  pane=panes.last();
  //layout=pane->layout;
}

// ---------------------------------------------------------------------
string Form::hschild()
{
  return p2s((void *)child);
}

// ---------------------------------------------------------------------
string Form::hsform()
{
  return p2s((void *)this);
}

// ---------------------------------------------------------------------
Child *Form::id2child(string n)
{
  for (int i=0; i<children.size(); i++)
    if (n==children.at(i)->id)
      return children.at(i);
  return (Child *) 0;
}

// ---------------------------------------------------------------------
bool Form::ischild(Child *n)
{
  return children.contains(n);
  //for (int i=0; i<children.size(); i++)
  //if (n==children.at(i))
  //return true;
  //return false;
}


// ---------------------------------------------------------------------
void Form::keyPressEvent(QKeyEvent *e)
{
  if (escclose && e->key()==Qt::Key_Escape) {
    e->ignore();
    delete this;
  }
}

// ---------------------------------------------------------------------
Child *Form::setmenuid(string id)
{
  if (menubar && menubar->items.contains(s2q(id)))
    return (Child *) menubar;
  return (Child *) 0;
}

// ---------------------------------------------------------------------
void Form::setpn(string p)
{
  setWindowTitle(s2q(p));
}

// ---------------------------------------------------------------------
void Form::showit()
{
  for (int i=panes.size()-1; i>=0; i--)
    panes.last()->fini();
  layout->addWidget(pane);
  setLayout(layout);
  show();
  shown=true;
}

// ---------------------------------------------------------------------
void Form::signalevent(Child *c)
{
  if (NoEvents || closed) return;
  string loc = locale;
  evtform=this;
  evtchild=c;
  if (c) {
    c->setform();
    sysmodifiers=c->sysmodifiers;
    sysdata=c->sysdata;
    loc = (""!=c->locale)?c->locale:locale;
  } else {
  }
  string s="(i.0 0)\"_ wdhandler_" + loc + "_$0";
  jedo((char *)s.c_str());
}

// ---------------------------------------------------------------------
string Form::state(int evt)
{
  string c,c1,e,r,s,ec;

  if (evt) {
    if (evtchild) {
      c=evtchild->id;
      c1=c+"_";
      e=evtchild->event;
      ec=evtchild->locale;
    } else
      e=event;
    r+=spair("syshandler",id+"_handler");
    r+=spair("sysevent",id+"_"+c1+e);
    r+=spair("sysdefault",id+"_default");
    r+=spair("sysparent",id);
    r+=spair("syschild",c);
    r+=spair("systype",e);
    r+=spair("syslocalec",ec);
  }

  // need only syslocale (not syslocalep, syslocalec)?...  in isigraph
  r+=spair("syslocalep",locale);
  r+=spair("syshwndp",hsform());
  r+=spair("syshwndc",hschild());
  r+=spair("syslastfocus","");
  r+=spair("sysmodifiers",sysmodifiers);
  r+=spair("sysdata",sysdata);

  for (int i=0; i<children.size(); i++)
    s+=children.at(i)->state();

  return r+s;
}
