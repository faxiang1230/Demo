/*
 * Copyright (c) 2013-2014 Conformal Systems <info@conformal.com>
 *
 * This file originated from: http://opensource.conformal.com/
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

package main

import (
	_"fmt"
	"log"

	"github.com/gotk3/gotk3/gtk"
)

func setup_window(title string) *gtk.Window {
	win, err := gtk.WindowNew(gtk.WINDOW_TOPLEVEL)
	if err != nil {
		log.Fatal("Unable to create window:", err)
	}
	win.SetTitle(title)
	win.Connect("destroy", func() {
		gtk.MainQuit()
	})
	win.SetDefaultSize(200, 50)
	win.SetPosition(gtk.WIN_POS_CENTER)
	win.SetResizable(false)
	return win
}

func setup_box(orient gtk.Orientation) *gtk.Box {
	box, err := gtk.BoxNew(orient, 0)
	if err != nil {
		log.Fatal("Unable to create box:", err)
	}
	return box
}
func setup_dialog() *gtk.Dialog {
	box, err := gtk.BoxNew(orient, 0)
	if err != nil {
		log.Fatal("Unable to create box:", err)
	}
	return box
}

func setup_label(msg string) *gtk.Label {
	l, err := gtk.LabelNew(msg)
	if err != nil {
		log.Fatal("Unable to create Label:", err)
	}
	return l
}


func main() {
	gtk.Init(nil)

	win := setup_window("志翔标密文件分析")
	box := setup_box(gtk.ORIENTATION_VERTICAL)
	win.Add(box)
	
	label := setup_label("你违规了")
	box.Add(label)
	//label := setup_label("你违规了")
	//label.SetText("你违规了")
/*
	tv := setup_tview()
	set_text_in_tview(tv, "Hello there!")
	box.PackStart(tv, true, true, 0)

	btn := setup_btn("Submit", func() {
		text := get_text_from_tview(tv)
		fmt.Println(text)
	})
	box.Add(btn)
*/
	// Recursively show all widgets contained in this window.
	win.ShowAll()

	// Begin executing the GTK main loop.  This blocks until
	// gtk.MainQuit() is run.
	gtk.Main()
}