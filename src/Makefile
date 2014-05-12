
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

GENOBJS=\
$(OUTDIR)/chtab.obj \
$(OUTDIR)/dumpver.obj \
$(OUTDIR)/msgdef.obj

BUILD=xyzzy.exe

!IF "$(VCVER)" == ""
!ERROR `VCVER' undeclared.
!ELSEIF "$(VCVER)" == "4"
CXXOPT=-Gf -GX
!ELSEIF "$(VCVER)" == "5" || "$(VCVER)" == "6"
CXXOPT=-GF -EHa
!ELSE
CXXOPT=-GF -EHa -Zc:wchar_t-
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
EDITBIN_OPTS=/largeaddressaware
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
CXXFLAGS=-Od -Gz -Gs -Gy -Gm $(CXXOPT) -W3 -MTd -Zi -Y- $(INC) $(DEFS) -DDEBUG -Fo$(OUTDIR)/
LDFLAGS=-debug -stack:0x2000000 -incremental:yes -nxcompat -safeseh
LIBS=$(LIBS) zlib/Debug/zlib.lib
FINAL_TARGET=$(OUTDIR)/$(BUILD)

!ELSE

OUTDIR=r
OBJS=$(OBJS: = r/)
CXXFLAGS=-Ob1 -Oi -Ot -Oy -Gs -Gy -Gz $(CXXOPT) -W3 -MT -vmb -Fp$(OUTDIR)/xyzzy.pch $(INC) $(DEFS) -Fo$(OUTDIR)/
LDFLAGS=-stack:0x2000000 -map:$(OUTDIR)/xyzzy.map -incremental:no -fixed -opt:ref -nxcompat -safeseh
LIBS=$(LIBS) zlib/Release/zlib.lib
FINAL_TARGET=../$(BUILD)

!ENDIF

.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -Yustdafx.h -c -TP $<

{gen/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -c -TP $<

{$(PRIVCTRLDIR)/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -c -TP $<

{$(DSFMTDIR)/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -c -TP $<

.rc.res:
	rc -r $<

.d{gen/}.h:
	$(MAKE) gen\gen-src1.exe
	.\gen\gen-src1 dpp $< "$@"

.asm{$(OUTDIR)/}.obj:
	$(ASM) $(ASMFLAGS) -c /Fo$@ $<

all: $(OUTDIR) $(OUTDIR)/xyzzy.pch gen/gen-src1.exe includes generates ../xyzzyenv.exe \
     $(FINAL_TARGET) ../xyzzycli.exe libxpi.lib

$(OUTDIR)/xyzzy.pch: stdafx.cc stdafx.h stdint_compat.h
	$(CXX) $(CXXFLAGS) -Ycstdafx.h -c -Tpstdafx.cc

includes: gen/vars-decl.h gen/fns-decl.h gen/msgcode.h
generates: gen/chtab.cc gen/dumpver.cc gen/msgdef.cc

$(OUTDIR)/$(BUILD): $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(GENOBJS) $(OUTDIR)/xyzzy.res
	$(LD) @<<
  $(LDFLAGS) -out:$@ $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(GENOBJS) -subsystem:windows $(LIBS) $(OUTDIR)/xyzzy.res
<<

$(OUTDIR): force
	-@if not exist $(OUTDIR)\nul mkdir $(OUTDIR)
gen:
	-@if not exist gen\nul mkdir gen

force:

GEN_SRC1_OBJS=\
gen\gen-syms.obj \
gen\gen-ctab.obj \
gen\gen-ktab.obj \
gen\gen-ucs2tab.obj \
gen\gen-utf2sjis.obj \
gen\gen-char-width.obj \
gen\gen-jisx0212-hash.obj \
gen\gen-jisx0212-width.obj \
gen\gen-lucida-width.obj \
gen\gen-fontrange.obj \
gen\gen-iso2022state.obj \
gen\gen-ibmext.obj \
gen\dpp.obj

GEN_SRC2_OBJS=\
gen\gen-msg.obj

clean:
	del $(OBJS:/=\) $(PRIVCTRLOBJS:/=\) $(DSFMTBOJS:/=\) $(GENOBJS:/=\)
	del $(OUTDIR)\$(BUILD) $(OUTDIR)\xyzzy.res $(OUTDIR)\xyzzy.rc \
	  $(OUTDIR)\$(BUILD:.exe=.map) $(OUTDIR)\$(BUILD:.exe=.ilk)
	del gen\gen-src1.obj gen\gen-src1.exe gen\gen-src2.obj gen\gen-src2.exe
	del $(GEN_SRC1_OBJS:/=\) $(GEN_SRC2_OBJS:/=\)
	del gen\num-arith.h

distclean: clean
	del $(OUTDIR)\xyzzy.pch
	del gen\vars-decl.h gen\fns-decl.h \
	  gen\symtable.cc gen\chtab.cc gen\msgcode.h \
	  gen\dumpver.cc gen\msgdef.cc gen\kanjitab.h gen\char-width.h \
	  gen\iso2022state.h gen\ibmext.h gen\utf2sjis.h gen\jisx0212-hash.h \
	  xdde.obj xyzzycli.obj xyzzycli.exe xyzzyenv.obj xyzzyenv.exe \
	  xyzzycli.res xyzzyenv.res libxpi.lib xpi.obj \
	  vc*.idb vc*.pdb *.aps XTAGS #* *~
	del gen\fontrange.h gen\lucida-width.h gen\version-describe.h \
	  gen\jisx0212-width.h gen\ucs2tab.h
	del $(PRIVCTRLDIR:/=\)\#* $(PRIVCTRLDIR:/=\)\*~

../$(BUILD): $(OUTDIR)/$(BUILD)
	$(EDITBIN) $(OUTDIR)\$(BUILD) $(SUBSYS_WINDOWS) $(EDITBIN_OPTS)
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

$(OUTDIR)/xyzzy.res: $(OUTDIR)/xyzzy.rc
	rc -r -fo$@ $(RCFLAGS) $(OUTDIR)/xyzzy.rc

$(OUTDIR)\xyzzy.rc: xyzzy.rc version-rc.h
	type xyzzy.rc > "$@"
	type version-rc.h >> "$@"

gen\version-describe.h: ..\misc\update_version_describe.bat gen
	..\misc\update_version_describe.bat

gen\vars-decl.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-syms -vars > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\fns-decl.h: gen/gen-src1.exe
	-@if exist "$@.tmp" del "$@" > nul
	.\gen\gen-src1 gen-syms -proto > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\symtable.cc: gen/gen-src1.exe
	-@if exist "$@.tmp" del "$@" > nul
	.\gen\gen-src1 gen-syms -symtable > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\dumpver.cc: gen/gen-src1.exe version.h
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-syms -version > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\chtab.cc: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-ctab > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\msgcode.h: gen/gen-src2.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src2 gen-msg -enum > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\msgdef.cc: gen/gen-src2.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src2 gen-msg -c > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\kanjitab.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-ktab > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\ucs2tab.h: ucs2tab.h
	copy ucs2tab.h gen
ucs2tab.h:
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-ucs2tab > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\utf2sjis.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-utf2sjis > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\char-width.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-char-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\jisx0212-width.h: jisx0212-width.h
	copy jisx0212-width.h gen
jisx0212-width.h:
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-jisx0212-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\jisx0212-hash.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-jisx0212-hash > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\lucida-width.h: lucida-width.h
	copy lucida-width.h gen
lucida-width.h:
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-lucida-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\iso2022state.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-iso2022state > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\ibmext.h: gen/gen-src1.exe
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-ibmext > "$@.tmp"
	move "$@.tmp" "$@" > nul

gen\fontrange.h: fontrange.h
	copy fontrange.h gen
fontrange.h:
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-fontrange > "$@.tmp"
	move "$@.tmp" "$@" > nul

rebuild-gen:
	del ucs2tab.h lucida-width.h jisx0212-width.h fontrange.h
	$(MAKE) ucs2tab.h lucida-width.h jisx0212-width.h fontrange.h

gen/gen-src1.obj: gen-main.cc gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-main.cc
gen/gen-src1.exe: gen/gen-src1.obj $(GEN_SRC1_OBJS)
	$(CXX) -Fe$@ gen/gen-src1.obj $(GEN_SRC1_OBJS) user32.lib gdi32.lib

gen/gen-src2.obj: gen-main.cc gen
	$(CXX) -MT -Fo$@ -c -DGEN_SRC2 -Tpgen-main.cc
gen/gen-src2.exe: gen/gen-src2.obj $(GEN_SRC2_OBJS)
	$(CXX) -MT -Fe$@ gen/gen-src2.obj $(GEN_SRC2_OBJS)

gen/gen-syms.obj: gen-syms.cc symbol.h function.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-syms.cc

gen/gen-ctab.obj: gen-ctab.cc chtype.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ctab.cc

gen/gen-msg.obj: gen-msg.cc gen/chtab.cc chtype.h msgdef.h gen
	$(CXX) -MT -Fo$@ -c -DGEN_SRC2 -Igen -Tpgen-msg.cc

gen/gen-ktab.obj: gen-ktab.cc gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ktab.cc

gen/gen-ucs2tab.obj: gen-ucs2tab.cc charset.h gen/fontrange.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ucs2tab.cc

gen/gen-utf2sjis.obj: gen-utf2sjis.cc gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-utf2sjis.cc

gen/gen-char-width.obj: gen-char-width.cc gen/jisx0212-width.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-char-width.cc

gen/gen-jisx0212-hash.obj: gen-jisx0212-hash.cc charset.h gen/jisx0212-width.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-jisx0212-hash.cc

gen/gen-jisx0212-width.obj: gen-jisx0212-width.cc gen/ucs2tab.h charset.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-jisx0212-width.cc

gen/gen-lucida-width.obj: gen-lucida-width.cc charset.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-lucida-width.cc

gen/gen-fontrange.obj: gen-fontrange.cc charset.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-fontrange.cc

gen/gen-iso2022state.obj: gen-iso2022state.cc gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-iso2022state.cc

gen/gen-ibmext.obj: gen-ibmext.cc gen/ucs2tab.h gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpgen-ibmext.cc

gen/dpp.obj: dpp.cc gen
	$(CXX) -Fo$@ -c -DGEN_SRC1 -Tpdpp.cc

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
$(OUTDIR)/disp.obj: syntaxinfo.h gen/lucida-width.h gen/jisx0212-hash.h binfo.h $(MAINFRAME_H) regex.h
$(OUTDIR)/doc.obj: except.h mman.h
$(OUTDIR)/dockbar.obj: dockbar.h
$(OUTDIR)/edict.obj: except.h mman.h
$(OUTDIR)/encdec.obj: byte-stream.h StrBuf.h encoding.h
$(OUTDIR)/encoding.obj: encoding.h gen/ibmext.h gen/utf2sjis.h
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
$(OUTDIR)/kanji.obj: gen/kanjitab.h safe_ptr.h byte-stream.h StrBuf.h encoding.h gen/iso2022state.h
$(OUTDIR)/kbd.obj: filer.h dialogs.h encoding.h
$(OUTDIR)/ldialog.obj: ldialog.h ColorDialog.h colors.h DnD.h glob.h $(PRIVCTRLDIR)/privctrl.h
$(OUTDIR)/lex.obj: lex.h
$(OUTDIR)/list.obj: sequence.h
$(OUTDIR)/listen.obj: listen.h
$(OUTDIR)/lprint.obj: StrBuf.h wstream.h sock.h
$(OUTDIR)/lread.obj: StrBuf.h
$(OUTDIR)/mman.obj: mman.h
$(OUTDIR)/number.obj: gen/num-arith.h
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
$(OUTDIR)/symtable.obj: symtable.cc gen/symtable.cc symtable.h
	$(CXX) $(CXXFLAGS) -c -Tpsymtable.cc
$(OUTDIR)/syntax.obj: syntaxinfo.h
$(OUTDIR)/thread.obj: thread.h
$(OUTDIR)/toplev.obj: ctl3d.h DnD.h environ.h fnkey.h $(MAINFRAME_H) wheel.h
$(OUTDIR)/usertab.obj: $(MAINFRAME_H) colors.h
$(OUTDIR)/usertool.obj: $(MAINFRAME_H)
$(OUTDIR)/ucs2.obj: gen/ucs2tab.h gen/char-width.h
$(OUTDIR)/vector.obj: sequence.h
$(OUTDIR)/version.obj: version.h gen/version-describe.h
$(OUTDIR)/wheel.obj: wheel.h
$(OUTDIR)/wstream.obj: StrBuf.h wstream.h
$(OUTDIR)/xdde.obj: xdde.h
$(OUTDIR)/resolver.obj: resolver.h sockimpl.h
$(OUTDIR)/sock.obj: sock.h resolver.h sockimpl.h
$(OUTDIR)/sockinet.obj: sockinet.h sock.h sockimpl.h

$(OBJS): $(ED_H)
