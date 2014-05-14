
# define VC++ version (4, 6-11)
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

GENDIR=./gen
GENOBJS=\
$(OUTDIR)/chtab.obj \
$(OUTDIR)/dumpver.obj \
$(OUTDIR)/msgdef.obj \
$(OUTDIR)/symtable.obj

ZLIBDIR=./zlib

BUILD=xyzzy.exe

!IF "$(VCVER)" == ""
!ERROR `VCVER' undeclared.
!ELSEIF "$(VCVER)" == "4"
CXXOPT=-Gf -GX
!ELSEIF "$(VCVER)" == "5"
!ERROR `VCVER=5' is not supported.
!ELSEIF "$(VCVER)" == "6"
CXXOPT=-GF -EHa
!ELSE
CXXOPT=-GF -EHa -Zc:wchar_t-
!ENDIF

DEFS=-DSTRICT -DWIN32 -D_WIN32 -D_MT -D__XYZZY__ -DPRIVCTRL_NODLL -DHAVE_DIMM_H -DDEBUG_GC
!IF "$(VCVER)" == "4" || "$(VCVER)" == "6" || "$(VCVER)" == "7"
CXX=cl -nologo
!ELSE
CXX=cl -nologo -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
!ENDIF
CPP=cl -nologo -E
ASM=ml -nologo
ASMFLAGS=-DVCVER=$(VCVER)
EDITBIN=editbin -nologo
EDITBIN_OPTS=
SUBSYS_WINDOWS=/SUBSYSTEM:WINDOWS,5.0
SUBSYS_CONSOLE=/SUBSYSTEM:CONSOLE,5.0
LD=link -nologo
LIBS=kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib \
comctl32.lib imm32.lib setargv.obj comdlg32.lib mpr.lib ole32.lib \
oleaut32.lib winspool.lib rpcrt4.lib wsock32.lib crypt32.lib secur32.lib
INC=-I$(PRIVCTRLDIR) -I$(GENDIR)

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
CXXFLAGS=-Od -Gz -Gs -Gy -Gm $(CXXOPT) -W3 -MTd -Zi -Fp$(OUTDIR)/xyzzy.pch $(INC) $(DEFS) -DDEBUG -Fo$(OUTDIR)/
LDFLAGS=-debug -stack:0x2000000 -incremental:yes -nxcompat
LIBS=$(LIBS) $(ZLIBDIR)/Debug/zlib.lib
FINAL_TARGET=$(OUTDIR)/$(BUILD)

!ELSE

OUTDIR=r
OBJS=$(OBJS: = r/)
CXXFLAGS=-Ob2 -O2 -Oi -Ot -Oy -Gs -Gy -Gz -GA $(CXXOPT) -W3 -MT -vmb -Fp$(OUTDIR)/xyzzy.pch $(INC) $(DEFS) -Fo$(OUTDIR)/
LDFLAGS=-stack:0x2000000 -map:$(OUTDIR)/xyzzy.map -incremental:no -fixed -opt:ref -nxcompat
LIBS=$(LIBS) $(ZLIBDIR)/Release/zlib.lib
FINAL_TARGET=../$(BUILD)

!ENDIF

.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -Yustdafx.h -c -TP $<

{$(GENDIR)/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -c -TP $<

{$(PRIVCTRLDIR)/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -Yustdafx.h -c -TP $<

{$(DSFMTDIR)/}.cc{$(OUTDIR)/}.obj::
	$(CXX) $(CXXFLAGS) -Yustdafx.h -c -TP $<

.rc.res:
	rc -r $<

.asm{$(OUTDIR)/}.obj:
	$(ASM) $(ASMFLAGS) -c /Fo$@ $<

all: $(OUTDIR) $(GENDIRSRCS) ../xyzzyenv.exe \
     $(FINAL_TARGET) ../xyzzycli.exe libxpi.lib

$(OUTDIR)/xyzzy.pch: stdafx.h targetver.h stdint_compat.h
	$(CXX) $(CXXFLAGS) -Ycstdafx.h -c -Tpstdafx.cc

$(OUTDIR)/$(BUILD): $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(GENOBJS) $(OUTDIR)/xyzzy.res
	$(LD) @<<
  $(LDFLAGS) -out:$@ $(OBJS) $(PRIVCTRLOBJS) $(DSFMTOBJS) $(GENOBJS) -subsystem:windows $(LIBS) $(OUTDIR)/xyzzy.res
<<

$(OUTDIR): force
	-@if not exist $(OUTDIR)\nul mkdir $(OUTDIR)
$(GENDIR): force
	-@if not exist $(GENDIR)\nul mkdir $(GENDIR)

force:

clean: gen-clean
	-del $(OBJS:/=\) $(PRIVCTRLOBJS:/=\) $(DSFMTOBJS:/=\) $(GENOBJS:/=\)
	-del $(OUTDIR)\$(BUILD) $(OUTDIR)\xyzzy.res $(OUTDIR)\xyzzy.rc \
	  $(OUTDIR)\$(BUILD:.exe=.map) $(OUTDIR)\$(BUILD:.exe=.ilk)

distclean: clean gen-distclean
	-del $(OUTDIR)\xyzzy.pch $(OUTDIR)\stdafx.obj
	-del xdde.obj xyzzycli.obj xyzzycli.exe xyzzyenv.obj xyzzyenv.exe \
	  xyzzycli.res xyzzyenv.res libxpi.lib xpi.obj \
	  vc*.idb vc*.pdb *.aps XTAGS #* *~
	-del $(PRIVCTRLDIR:/=\)\#* $(PRIVCTRLDIR:/=\)\*~

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
	$(CXX) -O2 -Zl -W3 -EHsc -c -Tpxpi.cc

$(OUTDIR)/xyzzy.res: $(OUTDIR)/xyzzy.rc
	rc -r -fo$@ $(RCFLAGS) $(OUTDIR)/xyzzy.rc

$(OUTDIR)\xyzzy.rc: xyzzy.rc version-rc.h
	type xyzzy.rc > "$@"
	type version-rc.h >> "$@"

# source code generators
GEN_SRC1_SRCS=\
gen-syms.cc \
gen-ctab.cc \
gen-ktab.cc \
gen-ucs2tab.cc \
gen-utf2sjis.cc \
gen-char-width.cc \
gen-jisx0212-hash.cc \
gen-jisx0212-width.cc \
gen-lucida-width.cc \
gen-fontrange.cc \
gen-iso2022state.cc \
gen-ibmext.cc \
dpp.cc

GEN_SRC2_SRCS=\
gen-msg.cc

GEN_SRC1_OBJS=$(SPC)$(GEN_SRC1_SRCS:.cc=.obj)
GEN_SRC1_OBJS=$(GEN_SRC1_OBJS:  = )
GEN_SRC2_OBJS=$(SPC)$(GEN_SRC2_SRCS:.cc=.obj)
GEN_SRC2_OBJS=$(GEN_SRC2_OBJS:  = )

!IF "$(GENDIR)" != "./gen"
!ERROR Please rewrite GEN_SRC1_OBJS, GEN_SRC2_OBJS and above line
!ENDIF
#GEN_SRC1_OBJS=$(GEN_SRC1_OBJS: = $^(GENDIR^)/)
GEN_SRC1_OBJS=$(GEN_SRC1_OBJS: = gen/)
#GEN_SRC2_OBJS=$(GEN_SRC2_OBJS: = $^(GENDIR^)/)
GEN_SRC2_OBJS=$(GEN_SRC2_OBJS: = gen/)

GEN_SRC1_EXE=$(GENDIR:/=\)\gen-src1.exe
GEN_SRC2_EXE=$(GENDIR:/=\)\gen-src2.exe

gen-clean:
	-del $(GENDIR:/=\)\gen-src1.obj $(GENDIR:/=\)\gen-src1.exe
	-del $(GENDIR:/=\)\gen-src2.obj $(GENDIR:/=\)\gen-src2.exe
	-del $(GEN_SRC1_OBJS:/=\) $(GEN_SRC2_OBJS:/=\)
	-del $(GENDIR:/=\)\num-arith.h

gen-distclean:
	-del $(GENDIR:/=\)\gen-stdafx.pch $(OUTDIR:/=\)\gen-stdafx.obj
	-del /q $(GENDIR:/=\)\*.*

.d{$(GENDIR)/}.h:
	$(MAKE) $(GEN_SRC1_EXE)
	$(GEN_SRC1_EXE) dpp "$<" "$@"

$(OUTDIR)/chtab.obj: $(GENDIR)/chtab.cc
$(OUTDIR)/dumpver.obj: $(GENDIR)/dumpver.cc
$(OUTDIR)/msgdef.obj: $(GENDIR)/msgdef.cc
$(OUTDIR)/symtable.obj: $(GENDIR)/symtable.cc

$(GENDIR)/version-describe.h: ..\misc\update_version_describe.bat
	..\misc\update_version_describe.bat

GENDIR_S=$(GENDIR:/=\)
$(GENDIR_S)\vars-decl.h: $(GEN_SRC1EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-syms -vars > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\fns-decl.h: $(GEN_SRC1_EXE)
	-@if exist "$@.tmp" del "$@" > nul
	$(GEN_SRC1_EXE) gen-syms -proto > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\symtable.cc: $(GEN_SRC1_EXE)
	-@if exist "$@.tmp" del "$@" > nul
	$(GEN_SRC1_EXE) gen-syms -symtable > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\dumpver.cc: $(GEN_SRC1_EXE) version.h
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-syms -version > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\chtab.cc: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	.\gen\gen-src1 gen-ctab > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\msgcode.h: $(GEN_SRC2_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC2_EXE) gen-msg -enum > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\msgdef.cc: $(GEN_SRC2_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC2_EXE) gen-msg -c > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\kanjitab.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-ktab > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\ucs2tab.h: ucs2tab.h
	copy ucs2tab.h "$@" > nul
ucs2tab.h:
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-ucs2tab > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\utf2sjis.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-utf2sjis > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\char-width.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-char-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\jisx0212-width.h: jisx0212-width.h
	copy jisx0212-width.h "$@" > nul
jisx0212-width.h:
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-jisx0212-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\jisx0212-hash.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-jisx0212-hash > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\lucida-width.h: lucida-width.h
	copy lucida-width.h "$@" >nul
lucida-width.h:
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-lucida-width > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\iso2022state.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-iso2022state > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\ibmext.h: $(GEN_SRC1_EXE)
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-ibmext > "$@.tmp"
	move "$@.tmp" "$@" > nul

$(GENDIR_S)\fontrange.h: fontrange.h
	copy fontrange.h "$@" > nul
fontrange.h:
	-@if exist "$@" del "$@" > nul
	$(GEN_SRC1_EXE) gen-fontrange > "$@.tmp"
	move "$@.tmp" "$@" > nul

rebuild-gen:
	$(MAKE) $(GEN_SRC1_EXE)
	del ucs2tab.h lucida-width.h jisx0212-width.h fontrange.h
	$(MAKE) ucs2tab.h lucida-width.h jisx0212-width.h fontrange.h

GEN_CXXFLAGS=-MT -Fp$(GENDIR)/gen-stdafx.pch -I$(GENDIR)
$(GENDIR)/gen-stdafx.pch: gen-stdafx.h targetver.h cdecl.h charset.h
	$(CXX) $(GEN_CXXFLAGS) -Fo$(GENDIR)/ -Ycgen-stdafx.h -c -Tpgen-stdafx.cc
.cc{$(GENDIR)/}.obj::
	$(CXX) $(GEN_CXXFLAGS) -Fo$(GENDIR)/ -Yugen-stdafx.h -c -TP $<

$(GEN_SRC1_EXE): $(GENDIR)/gen-src1.obj $(GEN_SRC1_OBJS)
	$(CXX) -MT -Fe$@ $(GENDIR)/gen-src1.obj $(GEN_SRC1_OBJS) user32.lib gdi32.lib

$(GEN_SRC2_EXE): $(GENDIR)/gen-src2.obj $(GEN_SRC2_OBJS)
	$(CXX) -MT -Fe$@ $(GENDIR)/gen-src2.obj $(GEN_SRC2_OBJS)

$(GENDIR)/gen-src1.obj: gen-main.cc
	$(CXX) -MT -Fo$@ -c -DGEN_SRC1 -Tpgen-main.cc

$(GENDIR)/gen-src2.obj: gen-main.cc
	$(CXX) -MT -Fo$@ -c -DGEN_SRC2 -Tpgen-main.cc

$(GENDIR)/gen-syms.obj: symbol.h function.h
$(GENDIR)/gen-ctab.obj: chtype.h
$(GENDIR)/gen-ktab.obj:
$(GENDIR)/gen-msg.obj: chtype.h msgdef.h $(GENDIR)/chtab.cc
$(GENDIR)/gen-ucs2tab.obj: $(GENDIR)/fontrange.h
$(GENDIR)/gen-char-width.obj: $(GENDIR)/jisx0212-width.h
$(GENDIR)/gen-jisx0212-hash.obj: $(GENDIR)/jisx0212-width.h
$(GENDIR)/gen-jisx0212-width.obj:$(GENDIR)/ucs2tab.h
$(GENDIR)/gen-ibmext.obj: $(GENDIR)/ucs2tab.h
$(GENDIR)/gen-lucida-width.obj:
$(GENDIR)/gen-utf2sjis.obj:
$(GENDIR)/gen-fontrange.obj:
$(GENDIR)/gen-iso2022state.obj:
$(GENDIR)/dpp.obj:
$(GEN_SRC1_OBJS) $(GEN_SRC2_OBJS): $(GENDIR)/gen-stdafx.pch

DEPENDTEMP=tmp.depend
depend: ../misc/makedep.py
	python ../misc/makedep.py ed.h mainframe.h    >  $(DEPENDTEMP)
	python ../misc/makedep.py $(SRCS)             >> $(DEPENDTEMP)
	python ../misc/makedep.py $(PRIVCTRLDIR)/*.cc >> $(DEPENDTEMP)
	python ../misc/makedep.py $(DSFMTDIR)/*.cc    >> $(DEPENDTEMP)
	move $(DEPENDTEMP) xyzzy.depend > nul

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
        -out:$@ $(OBJS)
        $(PRIVCTRLOBJS) $(DSFMTOBJS) $(GENOBJS) -profile $(LDFLAGS)
	-map:$(PROFDIR)/xyzzy.map -subsystem:windows $(LIBS) $(OUTDIR)/xyzzy.res
<<

profile: prof-dir prof-target
	@cd ..
	prep -nologo -OM -FT src/$(PROFDIR)/xyzzy
	profile -nologo src/$(PROFDIR)/xyzzy
	prep -nologo -M src/$(PROFDIR)/xyzzy
	plist -nologo -sc src/$(PROFDIR)/xyzzy


!INCLUDE "xyzzy.depend"
