#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# generated by wxGlade 0.7.1 on Fri Jun 15 21:10:57 2018
#

import wx
import os

# begin wxGlade: dependencies
import gettext
# end wxGlade

# begin wxGlade: extracode
# end wxGlade


class MyFrame(wx.Frame):
    def __init__(self, *args, **kwds):
        # begin wxGlade: MyFrame.__init__
        kwds["style"] = wx.DEFAULT_FRAME_STYLE
        wx.Frame.__init__(self, *args, **kwds)
        self.notebook_1 = wx.Notebook(self, wx.ID_ANY)
        self.notebook_1_pane_1 = wx.Panel(self.notebook_1, wx.ID_ANY)
        self.bitmap_button_1 = wx.BitmapButton(self.notebook_1_pane_1, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/schedule_clock-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_2 = wx.BitmapButton(self.notebook_1_pane_1, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/ram-card-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_3 = wx.BitmapButton(self.notebook_1_pane_1, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/dslr-camera-64.png", wx.BITMAP_TYPE_ANY))
        self.combo_box_1 = wx.ComboBox(self.notebook_1_pane_1, wx.ID_ANY, choices=[_("smaller"), _("medium"), _("large"), _("raw")], style=wx.CB_DROPDOWN | wx.CB_READONLY)
        self.bitmap_button_10 = wx.BitmapButton(self.notebook_1_pane_1, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/page_size-64.png", wx.BITMAP_TYPE_ANY))
        self.notebook_1_pane_2 = wx.Panel(self.notebook_1, wx.ID_ANY)
        self.bitmap_button_4 = wx.BitmapButton(self.notebook_1_pane_2, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/focus-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_5 = wx.BitmapButton(self.notebook_1_pane_2, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/button46-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_6 = wx.BitmapButton(self.notebook_1_pane_2, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/03_play-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_7 = wx.BitmapButton(self.notebook_1_pane_2, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/02_play-64.png", wx.BITMAP_TYPE_ANY))
        self.bitmap_button_8 = wx.BitmapButton(self.notebook_1_pane_2, wx.ID_ANY, wx.Bitmap("/home/leone/PIAstro/Live-64.png", wx.BITMAP_TYPE_ANY))
        self.slider_1 = wx.Slider(self.notebook_1_pane_2, wx.ID_ANY, 20, 2, 200, style=wx.SL_VALUE_LABEL | wx.SL_VERTICAL)

        self.__set_properties()
        self.__do_layout()

        self.Bind(wx.EVT_BUTTON, self.setDatePI, self.bitmap_button_1)
        self.Bind(wx.EVT_BUTTON, self.FreeRAM, self.bitmap_button_2)
        self.Bind(wx.EVT_BUTTON, self.setCCDInfo, self.bitmap_button_3)
        self.Bind(wx.EVT_BUTTON, self.setImageFormat, self.bitmap_button_10)
        self.Bind(wx.EVT_BUTTON, self.zeroFocuser, self.bitmap_button_4)
        self.Bind(wx.EVT_BUTTON, self.resetFocuser, self.bitmap_button_5)
        self.Bind(wx.EVT_BUTTON, self.FocusIN, self.bitmap_button_6)
        self.Bind(wx.EVT_BUTTON, self.FocusOUT, self.bitmap_button_7)
        self.Bind(wx.EVT_BUTTON, self.liveView, self.bitmap_button_8)
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: MyFrame.__set_properties
        self.SetTitle(_("PIAstro"))
        self.SetSize((160, 350))
        self.bitmap_button_1.SetToolTip(wx.ToolTip(_("Set Date&Time on Raspberry")))
        self.bitmap_button_1.SetSize(self.bitmap_button_1.GetBestSize())
        self.bitmap_button_2.SetToolTip(wx.ToolTip(_("Free Ram on Raspberry after LV")))
        self.bitmap_button_2.SetSize(self.bitmap_button_2.GetBestSize())
        self.bitmap_button_3.SetToolTip(wx.ToolTip(_("Set DSLR CCD Info on INDI")))
        self.bitmap_button_3.SetSize(self.bitmap_button_3.GetBestSize())
        self.combo_box_1.SetSelection(0)
        self.bitmap_button_10.SetToolTip(wx.ToolTip(_("Set Image format")))
        self.bitmap_button_10.SetSize(self.bitmap_button_10.GetBestSize())
        self.bitmap_button_4.SetToolTip(wx.ToolTip(_("Set focuser to 0")))
        self.bitmap_button_4.SetSize(self.bitmap_button_4.GetBestSize())
        self.bitmap_button_5.SetToolTip(wx.ToolTip(_("Reset focuser")))
        self.bitmap_button_5.SetSize(self.bitmap_button_5.GetBestSize())
        self.bitmap_button_6.SetToolTip(wx.ToolTip(_("Focus IN")))
        self.bitmap_button_6.SetSize(self.bitmap_button_6.GetBestSize())
        self.bitmap_button_7.SetToolTip(wx.ToolTip(_("Focus OUT")))
        self.bitmap_button_7.SetSize(self.bitmap_button_7.GetBestSize())
        self.bitmap_button_8.SetToolTip(wx.ToolTip(_("Liveview")))
        self.bitmap_button_8.SetSize(self.bitmap_button_8.GetBestSize())
        self.slider_1.SetToolTip(wx.ToolTip(_("Set focuser step")))
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: MyFrame.__do_layout
        sizer_1 = wx.BoxSizer(wx.VERTICAL)
        grid_sizer_1 = wx.GridSizer(3, 2, 0, 0)
        sizer_2 = wx.BoxSizer(wx.VERTICAL)
        sizer_3 = wx.BoxSizer(wx.VERTICAL)
        sizer_4 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_2.Add(self.bitmap_button_1, 0, wx.ALL, 0)
        sizer_2.Add(self.bitmap_button_2, 0, wx.ALL | wx.SHAPED, 0)
        sizer_2.Add(self.bitmap_button_3, 0, wx.ALL | wx.SHAPED, 0)
        sizer_4.Add(self.combo_box_1, 0, wx.ALIGN_CENTER_VERTICAL | wx.LEFT, 0)
        sizer_4.Add(self.bitmap_button_10, 0, wx.ALIGN_CENTER_VERTICAL | wx.ALL, 0)
        sizer_3.Add(sizer_4, 1, 0, 0)
        sizer_2.Add(sizer_3, 1, 0, 0)
        self.notebook_1_pane_1.SetSizer(sizer_2)
        grid_sizer_1.Add(self.bitmap_button_4, 0, wx.ALL, 0)
        grid_sizer_1.Add(self.bitmap_button_5, 0, wx.ALL, 0)
        grid_sizer_1.Add(self.bitmap_button_6, 0, wx.ALL, 0)
        grid_sizer_1.Add(self.bitmap_button_7, 0, wx.ALL, 0)
        grid_sizer_1.Add(self.bitmap_button_8, 0, wx.ALL, 0)
        grid_sizer_1.Add(self.slider_1, 0, wx.ALL | wx.EXPAND, 0)
        self.notebook_1_pane_2.SetSizer(grid_sizer_1)
        self.notebook_1.AddPage(self.notebook_1_pane_1, _("Settings"))
        self.notebook_1.AddPage(self.notebook_1_pane_2, _("Focuser"))
        sizer_1.Add(self.notebook_1, 1, wx.EXPAND, 0)
        self.SetSizer(sizer_1)
        self.Layout()
        # end wxGlade

    def setDatePI(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/setPiAstroDate.sh")

    def FreeRAM(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/liveFreeMem.sh")

    def setCCDInfo(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/setCCD_DSRL.sh")

    def setImageFormat(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/setImageDim.sh "+self.combo_box_1.GetValue())

    def zeroFocuser(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/setFocuserPosition.sh 0")

    def resetFocuser(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/resetFocuser.sh")

    def FocusIN(self, event):  # wxGlade: MyFrame.<event_handler>
        print "Focus IN by "+str(self.slider_1.GetValue())
        os.system("~/PIAstro/Focus.sh IN "+str(self.slider_1.GetValue()))

    def FocusOUT(self, event):  # wxGlade: MyFrame.<event_handler>
        print "Focus OUT by "+str(self.slider_1.GetValue())
        os.system("~/PIAstro/Focus.sh OUT "+str(self.slider_1.GetValue()))

    def liveView(self, event):  # wxGlade: MyFrame.<event_handler>
        os.system("~/PIAstro/grabLiveView.sh")

# end of class MyFrame
class MyApp(wx.App):
    def OnInit(self):
        PIAstro = MyFrame(None, wx.ID_ANY, "")
        self.SetTopWindow(PIAstro)
        PIAstro.Show()
        return True

# end of class MyApp

if __name__ == "__main__":
    gettext.install("PIAstro") # replace with the appropriate catalog name

    PIAstro = MyApp(0)
    PIAstro.MainLoop()
