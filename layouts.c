void 
magicgrid(Monitor *m) {
  Client *c;
  int total = 0;

	for(c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
    total++;
  }
  
  int rows;
  for (rows = 1; rows < total; rows++) {
    int rowssq = rows * rows;
    int nextsq = (rows + 1) * (rows + 1);

    if (total >= rowssq && total < nextsq) {
      break; 
    }
  }

  int basecols = total / rows;
  int extendedrows = total - (rows * basecols);

  c = nexttiled(m->clients);

  for (int row = 0; row < rows; row++) {
    int cols = (row >= rows - extendedrows) ? basecols + 1: basecols;

    for (int col = 0; col < cols; col++) {
      int w = (m->ww - gappx) / cols;
      int h = (m->wh - gappx) / rows;
      int x = m->wx + col * w + gappx;
      int y = m->wy + row * h + gappx;

      int buff = (2 * c->bw) + gappx;

      resize(c, x, y, w - buff, h - buff, 0);
      c = nexttiled(c->next);
    }
  }
}

void
movestack(const Arg *arg) {
	Client *c = NULL, *p = NULL, *pc = NULL, *i;

	if(arg->i > 0) {
		/* find the client after selmon->sel */
		for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
		if(!c)
			for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);

	}
	else {
		/* find the client before selmon->sel */
		for(i = selmon->clients; i != selmon->sel; i = i->next)
			if(ISVISIBLE(i) && !i->isfloating)
				c = i;
		if(!c)
			for(; i; i = i->next)
				if(ISVISIBLE(i) && !i->isfloating)
					c = i;
	}
	/* find the client before selmon->sel and c */
	for(i = selmon->clients; i && (!p || !pc); i = i->next) {
		if(i->next == selmon->sel)
			p = i;
		if(i->next == c)
			pc = i;
	}

	/* swap c and selmon->sel selmon->clients in the selmon->clients list */
	if(c && c != selmon->sel) {
		Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
		selmon->sel->next = c->next==selmon->sel?c:c->next;
		c->next = temp;

		if(p && p != c)
			p->next = c;
		if(pc && pc != selmon->sel)
			pc->next = selmon->sel;

		if(selmon->sel == selmon->clients)
			selmon->clients = c;
		else if(c == selmon->clients)
			selmon->clients = selmon->sel;

		arrange(selmon);
	}
}

