
# define VC++ version (4, 6, or 7)
VCVER=9

.SUFFIXES: .cc .rc .res .d .asm

SRCS=\
Buffer.cc \
ChooseFont.cc \
ColorDialog.cc \
DnD.cc \
StrBuf.cc \
Window.cc \
alloc.cc \
appid.cc \
arc-if.cc \
archiver.cc \
array.cc \
assert.cc \
backq.cc \
bignum.cc \
binfo.cc \
buffer-bar.cc \
byte-stream.cc \
bytecode.cc \
ces.cc \
char.cc \
chname.cc \
chtab.cc \
clipboard.cc \
com.cc \
conf.cc \
colors.cc \
ctl3d.cc \
chunk.cc \
data.cc \
dde.cc \
debug.cc \
dialogs.cc \
disp.cc \
dll.cc \
doc.cc \
dockbar.cc \
dumpver.cc \
edict.cc \
encdec.cc \
encoding.cc \
environ.cc \
eval.cc \
except.cc \
fileio.cc \
filer.cc \
fnkey.cc \
font.cc \
frame.cc \
gime.cc \
glob.cc \
guess.cc \
guid.cc \
gzip.cc \
hash.cc \
hashpjw.cc \
init.cc \
insdel.cc \
ipc.cc \
kanji.cc \
kbd.cc \
keymap.cc \
ldialog.cc \
lex.cc \
list.cc \
listen.cc \
lprint.cc \
lread.cc \
md5c.cc \
menu.cc \
minibuf.cc \
mman.cc \
monitor.cc \
mouse.cc \
move.cc \
msgbox.cc \
msgdef.cc \
number.cc \
oledata.cc \
package.cc \
pane.cc \
pathname.cc \
popup.cc \
popupl.cc \
pred.cc \
preview.cc \
print.cc \
printdlg.cc \
process.cc \
random.cc \
regex.cc \
resolver.cc \
search.cc \
sequence.cc \
sha1.cc \
sha2.cc \
signal.cc \
sock.cc \
sockinet.cc \
sockssl.cc \
statarea.cc \
stdctl.cc \
stream.cc \
string.cc \
structure.cc \
symbol.cc \
symtable.cc \
syntax.cc \
sysdep.cc \
system.cc \
thread.cc \
toplev.cc \
ucs2.cc \
undo.cc \
unzip.cc \
usertab.cc \
usertool.cc \
utils.cc \
utimer.cc \
vc-compat.cc \
vector.cc \
version.cc \
vfs.cc \
wheel.cc \
winhelp.cc \
wstream.cc \
xdde.cc \
xlist.cc

ASMS=\
vc-compat-entry.asm

PRIVCTRLDIR=./privctrl
PRIVCTRLOBJS=\
$(OUTDIR)/privctrl.obj \
$(OUTDIR)/listviewex.obj \
$(OUTDIR)/url.obj

DSFMTDIR=./dsfmt
DSFMTOBJS=\
$(OUTDIR)/dSFMT.obj

BUILD=xyzzy.exe

!IF "$(VCVER)" == ""
!ERROR `VCVER' undeclared.
!ELSEIF "$(VCVER)" == "4"
CXXOPT=-Gf -GX
!ELSE
CXXOPT=-GF -EHa
!ENDIF

DEFS=-DSTRICT -DWIN32 -D_WIN32 -D_MT -D__XYZZY__ -DPRIVCTRL_NODLL -DHAVE_DIMM_H -DDEBUG_GC
!IF "$(VCVER)" == "8" || "$(VCVER)" == "9" || "$(VCVER)" == "10" || "$(VCVER)" == "11"
CXX=cl -nologo -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
!ELSE
CXX=cl -nologo
!ENDIF
CPP=cl -nologo -E
ASM=ml -nologo
ASMFLAGS=-DVCVER=$(VCVER)
EDITBIN=editbin -nologo
SUBSYS_WINDOWS=/SUBSYSTEM:WINDOWS,5.0
SUBSYS_CONSOLE=/SUBSYSTEM:CONSOLE,5.0
LD=link -nologo
LIBS=kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib \
comctl32.lib imm32.lib setargv.obj comdlg32.lib mpr.lib ole32.lib \
oleaut32.lib winspool.lib rpcrt4.lib wsock32.lib crypt32.lib secur32.lib
INC=-I$(PRIVCTRLDIR) -I./gen

SPC=""
SPC=$(SPC:""= )

OBJS=$(SPC)$(SRCS:.cc=.obj) $(ASMS:.asm=.obj)
OBJS=$(OBJS:  = )

!IFNDEF CFG
CFG=r
!ENDIF

!IF "$(CFG)" == "d"

OUTDIR=d
OBJS=$(OBJS: = d/)
CXXFLAGS=-Od -Gz -Gs -Gy -Gm $(CXXOPT) -W3 -MTd -Zi $(INC) $(DEFS) -DDEBUG -Fo$(OUTDIR)/
LDFLAGS=-debug -stack:0x2000000 -incremental:yes
LIBS=$(LIBS) zlib/Debug/zlib.lib
FINAL_TARGET=$(OUTDIR)/$(BUILD)

!ELSE

OUTDIR=r
OBJS=$(OBJS: = r/)
CXXFLAGS=-Ob1 -Oi -Ot -Oy -Gs -Gy -Gz $(CXXOPT) -W3 -MT -vmb $(INC) $(DEFS) -Fo$(OUTDIR)/
LDFLAGS=-stack:0x2000000 -map:$(OUTDIR)/xyzzy.map -incremental:no -fixed -opt:ref
LIBS=$(LIBS) zlib/Release/zlib.lib
FINAL_TARGET=../$(BUILD)

!ENDIF

.cc{$(OUTDIR)/}.obj:
	$(CXX) $(CXXFLAGS) -c -Tp$<

{$(PRIVCTRLDIR)/}.cc{$(OUTDIR)/}.obj:
	$(CXX) $(CXXFLAGS) -c -Tp$<

{$(DSFMTDIR)/}.cc{$(OUTDIR)/}.obj:
	$(CXX) $(CXXFLAGS) -c -Tp$<

.rc.res:
	rc -r $<

.d.h:
	.\gen-src1 dpp $< $@

.asm{$(OUTDIR)/}.obj:
	$(ASM) $(ASMFLAGS) -c /Fo$@ $<

all: $(OUTDIR) gen-src1.exe includes ../xyzzyenv.exe \
     $(FINAL_TARGET) ../xyzzycli.exe libxpi.lib

includes: vars-decl.h fns-decl.h msgcode.h

$(OUTDIR)/$(BUILD): $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(OUTDIR)/xyzzy.res
	$(LD) @<<
  $(LDFLAGS) -out:$@ $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) -subsystem:windows $(LIBS) $(OUTDIR)/xyzzy.res
<<

$(OUTDIR): force
	-@if not exist $(OUTDIR)\nul mkdir $(OUTDIR)

force:

clean:
	rm -f $(OBJS:/=\) $(PRIVCTRLOBJS:/=\) $(DSFMTBOJS:/=\) $(OUTDIR)\$(BUILD)
	rm -f $(OUTDIR)\xyzzy.res $(OUTDIR)\xyzzy.rc $(OUTDIR)\$(BUILD:.exe=.map) \
	  $(OUTDIR)\$(BUILD:.exe=.ilk)
	rm -f gen-src1.obj gen-src1.exe gen-src2.obj gen-src2.exe
	rm -f gen-syms.obj gen-ctab.obj
	rm -f gen-ktab.obj gen-msg.obj
	rm -f gen-ucs2tab.obj
	rm -f gen-utf2sjis.obj
	rm -f gen-char-width.obj
	rm -f gen-jisx0212-hash.obj
	rm -f gen-iso2022state.obj
	rm -f gen-ibmext.obj
	rm -f dpp.obj num-arith.h

distclean: clean
	rm -f vars-decl.h fns-decl.h symtable.gen.cc chtab.gen.cc msgcode.h \
	  dumpver.gen.cc msgdef.gen.cc kanjitab.h char-width.h \
	  gen-jisx0212-width.obj gen-jisx0212-width.exe \
	  gen-lucida-width.obj gen-lucida-width.exe \
	  gen-fontrange.obj gen-fontrange.exe \
	  iso2022state.h ibmext.h utf2sjis.h jisx0212-hash.h \
	  xdde.obj xyzzycli.obj xyzzycli.exe xyzzyenv.obj xyzzyenv.exe \
	  xyzzycli.res xyzzyenv.res libxpi.lib xpi.obj \
	  vc40.idb vc40.pdb *.aps XTAGS #* *~ \
	  $(PRIVCTRLDIR)/#* $(PRIVCTRLDIR)/*~

realclean: distclean
	rm -f jisx0212-width.h lucida-width.h ucs2tab.h fontrange.h

../$(BUILD): $(OUTDIR)/$(BUILD)
	$(EDITBIN) $(OUTDIR)\$(BUILD) $(SUBSYS_WINDOWS)
	copy $(OUTDIR)\$(BUILD) .. > nul
	copy $(OUTDIR)\xyzzy.map .. > nul

../xyzzyenv.exe: xyzzyenv.exe
	$(EDITBIN) xyzzyenv.exe $(SUBSYS_CONSOLE)
	copy xyzzyenv.exe .. > nul

xyzzyenv.exe: xyzzyenv.cc xyzzyenv.res
	$(CXX) -O2 -Tpxyzzyenv.cc xyzzyenv.res kernel32.lib user32.lib \
	   -link -fixed -subsystem:console

../xyzzycli.exe: xyzzycli.exe
	$(EDITBIN) xyzzycli.exe $(SUBSYS_WINDOWS)
	copy xyzzycli.exe .. > nul

CLIENT_OBJS=xyzzycli.obj
xyzzycli.exe: $(CLIENT_OBJS) xyzzycli.res
	$(CXX) $(CLIENT_OBJS) kernel32.lib user32.lib xyzzycli.res \
	  -link -fixed -subsystem:windows
#	$(CXX) $(CLIENT_OBJS) setargv.obj kernel32.lib user32.lib xyzzycli.res

xyzzycli.obj: xyzzycli.cc xdde.h xyzzycli.h
	$(CXX) -O2 -W3 -c -DSTRICT -Tpxyzzycli.cc

xyzzycli.res: xyzzycli.h

libxpi.lib: xpi.obj
	lib -out:$@ xpi.obj

xpi.obj: xpi.cc
	$(CXX) -O2 -Zl -W3 -c -Tpxpi.cc

TMPFILE=tmpfile.$$$$

$(OUTDIR)/xyzzy.res: $(OUTDIR)/xyzzy.rc
	rc -r -fo$@ $(RCFLAGS) $(OUTDIR)/xyzzy.rc

$(OUTDIR)\xyzzy.rc: xyzzy.rc version-rc.h
	type xyzzy.rc > $@
	type version-rc.h >> $@

gen/version-describe.h:
	-@if not exist gen/nul mkdir gen
	..\misc\update_version_describe.bat

vars-decl.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-syms -vars > $(TMPFILE)
	move $(TMPFILE) $@ > nul

fns-decl.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-syms -proto > $(TMPFILE)
	move $(TMPFILE) $@ > nul

gen/symtable.cc: gen-src1.exe
	-@if not exist gen/nul mkdir gen
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-syms -symtable > $(TMPFILE)
	move $(TMPFILE) $@ > nul

dumpver.cc: gen-src1.exe version.h
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-syms -version > $(TMPFILE)
	move $(TMPFILE) $@ > nul

chtab.cc: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-ctab > $(TMPFILE)
	move $(TMPFILE) $@ > nul

msgcode.h: gen-src2.exe
	-@if exist $@ del $@ > nul
	.\gen-src2 gen-msg -enum > $(TMPFILE)
	move $(TMPFILE) $@ > nul

msgdef.cc: gen-src2.exe
	-@if exist $@ del $@ > nul
	.\gen-src2 gen-msg -c > $(TMPFILE)
	move $(TMPFILE) $@ > nul

kanjitab.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-ktab > $(TMPFILE)
	move $(TMPFILE) $@ > nul

ucs2tab.h:
	$(MAKE) gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-ucs2tab > $(TMPFILE)
	move $(TMPFILE) $@ > nul

utf2sjis.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-utf2sjis > $(TMPFILE)
	move $(TMPFILE) $@ > nul

char-width.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-char-width > $(TMPFILE)
	move $(TMPFILE) $@ > nul

jisx0212-width.h:
	$(MAKE) gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-jisx0212-width > $(TMPFILE)
	move $(TMPFILE) $@ > nul

jisx0212-hash.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-jisx0212-hash > $(TMPFILE)
	move $(TMPFILE) $@ > nul

lucida-width.h:
	$(MAKE) gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-lucida-width > $(TMPFILE)
	move $(TMPFILE) $@ > nul

iso2022state.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-iso2022state > $(TMPFILE)
	move $(TMPFILE) $@ > nul

ibmext.h: gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-ibmext > $(TMPFILE)
	move $(TMPFILE) $@ > nul

fontrange.h:
	$(MAKE) gen-src1.exe
	-@if exist $@ del $@ > nul
	.\gen-src1 gen-fontrange > $(TMPFILE)
	move $(TMPFILE) $@ > nul

GEN_SRC1_OBJS=\
gen-syms.obj \
gen-ctab.obj \
gen-ktab.obj \
gen-ucs2tab.obj \
gen-utf2sjis.obj \
gen-char-width.obj \
gen-jisx0212-hash.obj \
gen-jisx0212-width.obj \
gen-lucida-width.obj \
gen-fontrange.obj \
gen-iso2022state.obj \
gen-ibmext.obj \
dpp.obj

GEN_SRC2_OBJS=\
gen-msg.obj

gen-src1.obj: gen-main.cc
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-main.cc
gen-src1.exe: gen-src1.obj $(GEN_SRC1_OBJS)
	$(CXX) -Fe$@ gen-src1.obj $(GEN_SRC1_OBJS) user32.lib gdi32.lib
gen-src2.obj: gen-main.cc
	$(CXX) -MT -Fo$@ -c -DGEN_SRC2 -Tpgen-main.cc
gen-src2.exe: gen-src2.obj $(GEN_SRC2_OBJS)
	$(CXX) -MT -Fe$@ gen-src2.obj $(GEN_SRC2_OBJS)

gen-syms.obj: gen-syms.cc symbol.h function.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-syms.cc

gen-ctab.obj: gen-ctab.cc chtype.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ctab.cc

gen-msg.obj: gen-msg.cc chtab.cc chtype.h msgdef.h
	$(CXX) -MT -Fo$@ -c -DGEN_SRC2 -Tpgen-msg.cc

gen-ktab.obj: gen-ktab.cc
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ktab.cc

gen-ucs2tab.obj: gen-ucs2tab.cc charset.h fontrange.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ucs2tab.cc

gen-utf2sjis.obj: gen-utf2sjis.cc
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-utf2sjis.cc

gen-char-width.obj: gen-char-width.cc jisx0212-width.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-char-width.cc

gen-jisx0212-hash.obj: gen-jisx0212-hash.cc charset.h jisx0212-width.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-jisx0212-hash.cc

gen-jisx0212-width.obj: gen-jisx0212-width.cc ucs2tab.h charset.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-jisx0212-width.cc

gen-lucida-width.obj: gen-lucida-width.cc charset.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-lucida-width.cc

gen-fontrange.obj: gen-fontrange.cc charset.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-fontrange.cc

gen-iso2022state.obj: gen-iso2022state.cc
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-iso2022state.cc

gen-ibmext.obj: gen-ibmext.cc ucs2tab.h
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ibmext.cc

dpp.obj: dpp.cc
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpdpp.cc

ED_H=alloc.h array.h bignum.h Buffer.h cdecl.h char.h chtype.h chunk.h \
closure.h cons.h data.h dll.h ed.h error.h function.h hash.h \
kanji.h kbd.h lisp.h list.h message.h mouse.h msgbox.h \
number.h oledata.h package.h process.h random.h stream.h string.h \
structure.h symbol.h syntax.h sysdep.h timer.h trace.h reconv.h \
vector.h Window.h utils.h xcolor.h readtab.h vfs.h waitobj.h charset.h \
font.h ces.h gime.h statarea.h utimer.h xlist.h mousemsg.h
MAINFRAME_H=mainframe.h dockbar.h pane.h

$(OUTDIR)/Buffer.obj: syntaxinfo.h filer.h glob.h dialogs.h binfo.h $(MAINFRAME_H) buffer-bar.h DnD.h
$(OUTDIR)/ChooseFont.obj: ChooseFont.h
$(OUTDIR)/ColorDialog.obj: ldialog.h ChooseFont.h ColorDialog.h colors.h DnD.h conf.h glob.h
$(OUTDIR)/DnD.obj: $(PRIVCTRLDIR)/privctrl.h com.h dialogs.h DnD.h filer.h glob.h buffer-bar.h dockbar.h
$(OUTDIR)/StrBuf.obj: StrBuf.h
$(OUTDIR)/Window.obj: conf.h ipc.h wheel.h
$(OUTDIR)/arc-if.obj: arc-if.h comm-arc.h
$(OUTDIR)/archiver.obj: arc-if.h archiver.h except.h comm-arc.h
$(OUTDIR)/buffer-bar.obj: buffer-bar.h DnD.h $(MAINFRAME_H) colors.h
$(OUTDIR)/bytecode.obj: lex.h
$(OUTDIR)/byte-stream.obj: byte-stream.h StrBuf.h encoding.h
$(OUTDIR)/colors.obj: conf.h colors.h filer.h $(MAINFRAME_H)
$(OUTDIR)/com.obj: com.h
$(OUTDIR)/conf.obj: conf.h print.h wheel.h
$(OUTDIR)/ctl3d.obj: ctl3d.h
$(OUTDIR)/data.obj: dataP.h lex.h symtable.h $(MAINFRAME_H)
$(OUTDIR)/dde.obj: safe_ptr.h xdde.h
$(OUTDIR)/dialogs.obj: $(PRIVCTRLDIR)/privctrl.h dialogs.h conf.h thread.h xstrlist.h
$(OUTDIR)/disp.obj: syntaxinfo.h lucida-width.h jisx0212-hash.h binfo.h $(MAINFRAME_H) regex.h
$(OUTDIR)/doc.obj: except.h mman.h
$(OUTDIR)/dockbar.obj: dockbar.h
$(OUTDIR)/edict.obj: except.h mman.h
$(OUTDIR)/encdec.obj: byte-stream.h StrBuf.h encoding.h
$(OUTDIR)/encoding.obj: encoding.h ibmext.h utf2sjis.h
$(OUTDIR)/environ.obj: environ.h fnkey.h conf.h
$(OUTDIR)/eval.obj: lex.h
$(OUTDIR)/except.obj: except.h
$(OUTDIR)/fileio.obj: except.h mman.h encoding.h
$(OUTDIR)/filer.obj: $(PRIVCTRLDIR)/privctrl.h dialogs.h DnD.h environ.h filer.h glob.h conf.h colors.h
$(OUTDIR)/font.obj: font.h conf.h
$(OUTDIR)/fnkey.obj: fnkey.h
$(OUTDIR)/glob.obj: glob.h
$(OUTDIR)/init.obj: $(PRIVCTRLDIR)/privctrl.h ctl3d.h environ.h except.h fnkey.h xdde.h syntaxinfo.h ipc.h sock.h $(MAINFRAME_H)
$(OUTDIR)/insdel.obj: syntaxinfo.h sequence.h byte-stream.h StrBuf.h encoding.h
$(OUTDIR)/ipc.obj: ipc.h
$(OUTDIR)/kanji.obj: kanjitab.h safe_ptr.h byte-stream.h StrBuf.h encoding.h iso2022state.h
$(OUTDIR)/kbd.obj: filer.h dialogs.h encoding.h
$(OUTDIR)/ldialog.obj: ldialog.h ColorDialog.h colors.h DnD.h glob.h $(PRIVCTRLDIR)/privctrl.h
$(OUTDIR)/lex.obj: lex.h
$(OUTDIR)/list.obj: sequence.h
$(OUTDIR)/listen.obj: listen.h
$(OUTDIR)/lprint.obj: StrBuf.h wstream.h sock.h
$(OUTDIR)/lread.obj: StrBuf.h
$(OUTDIR)/mman.obj: mman.h
$(OUTDIR)/number.obj: num-arith.h
$(OUTDIR)/pathname.obj: dyn-handle.h environ.h pathname.h comm-arc.h except.h mman.h thread.h xstrlist.h
$(OUTDIR)/pane.obj: $(MAINFRAME_H)
$(OUTDIR)/pred.obj: lex.h
$(OUTDIR)/preview.obj: conf.h print.h preview.h wheel.h
$(OUTDIR)/print.obj: print.h lucida-width.h preview.h wheel.h
$(OUTDIR)/printdlg.obj: print.h printdlg.h
$(OUTDIR)/process.obj: dyn-handle.h byte-stream.h StrBuf.h sock.h sockinet.h encoding.h $(MAINFRAME_H)
$(OUTDIR)/regex.obj: regex.h
$(OUTDIR)/search.obj: regex.h
$(OUTDIR)/sequence.obj: safe_ptr.h sequence.h
$(OUTDIR)/stream.obj: sequence.h StrBuf.h wstream.h sock.h sockinet.h
$(OUTDIR)/string.obj: sequence.h StrBuf.h
$(OUTDIR)/symtable.obj: gen/symtable.cc symtable.h
$(OUTDIR)/syntax.obj: syntaxinfo.h
$(OUTDIR)/thread.obj: thread.h
$(OUTDIR)/toplev.obj: ctl3d.h DnD.h environ.h fnkey.h $(MAINFRAME_H) wheel.h
$(OUTDIR)/usertab.obj: $(MAINFRAME_H) colors.h
$(OUTDIR)/usertool.obj: $(MAINFRAME_H)
$(OUTDIR)/ucs2.obj: ucs2tab.h char-width.h
$(OUTDIR)/vector.obj: sequence.h
$(OUTDIR)/version.obj: version.h gen/version-describe.h
$(OUTDIR)/wheel.obj: wheel.h
$(OUTDIR)/wstream.obj: StrBuf.h wstream.h
$(OUTDIR)/xdde.obj: xdde.h
$(OUTDIR)/resolver.obj: resolver.h sockimpl.h
$(OUTDIR)/sock.obj: sock.h resolver.h sockimpl.h
$(OUTDIR)/sockinet.obj: sockinet.h sock.h sockimpl.h

$(OBJS): $(ED_H)

depend:
	set CC=$(CXX)
	set CFLAGS=$(CXXFLAGS)
	mkdep $(SRCS:.cc=)

.cc.cod:
	$(CXX) $(CXXFLAGS) -FAcs -c -Tp$<
.cc.asm:
	$(CXX) $(CXXFLAGS) -Fa -c -Tp$<


PROFDIR=$(OUTDIR)\prof

prof-dir: force
	-@if not exist $(PROFDIR)\nul mkdir $(PROFDIR)

prof-target: $(PROFDIR)\$(BUILD)

$(PROFDIR)/$(BUILD): $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(OUTDIR)/xyzzy.res
	$(LD) @<<
  -out:$@ $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) -profile -stack:0x2000000
	-map:$(PROFDIR)/xyzzy.map -subsystem:windows $(LIBS) $(OUTDIR)/xyzzy.res
<<

profile: prof-dir prof-target
	@cd ..
	prep -nologo -OM -FT src/$(PROFDIR)/xyzzy
	profile -nologo src/$(PROFDIR)/xyzzy
	prep -nologo -M src/$(PROFDIR)/xyzzy
	plist -nologo -sc src/$(PROFDIR)/xyzzy

