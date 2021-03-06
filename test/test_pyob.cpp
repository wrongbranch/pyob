/*
  test_pyob.cpp
*/

//#ifdef _CONSOLE || _WINDOWS
#include "stdafx.h"
//#endif

#include "pyob.h"

void test_ide(void){
try{
/*
  char *fn = "_test_.py"; // UTF-8
//PyRun_SimpleFile(); // not for Windows (may be caused FileSystemEncoding ?)
  PyObject* py_fo = PyFile_FromString(fn, "r");
  PyRun_SimpleFile(PyFile_AsFile(py_fo), fn);
*/
/*
  PyRun_SimpleString("TEST_GLOBAL = b'\\xe9\\xa1\\xb1'.decode('utf-8')");
  const wchar_t *argv[] = {L"fake_main.exe", L"_test_executed_.py"}; // , L""
  Py_Main(sizeof(argv) / sizeof(argv[0]) - 1, (wchar_t **)argv); // IDE after W
*/
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_base(void){
try{
  pyob::PyObj o;
  pyob::PyObj p = o; // copy
  pyob::PyObj q = pyob::PyObj(); // not copy
  pyob::PyObj &r = o;
  pyob::PyTpl t = pyob::PyTpl();
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_item(void){
try{
  pyob::PyLng a(3);
  pyob::PyLng b(5);
  pyob::PyLng c(7);
  pyob::PyLst lst(std::tie(a, b, c));
  pyob::PyTpl tpl(std::tie(a, b, c));
  pyob::PyDct dct({{"X", PYLNG(123)}, {"Y", PYDBL(4.56)}, {"Z", PYSTR("789")}});

PYREPR(stdout, lst);
PYREPR(stdout, lst[1]);
  lst[1] = PYLNG(-10);
  try{
    lst[9] = PYLNG(-99); // SKIP: exception - invalid index
  }catch(const std::exception &e){
    fprintf(stderr, "SKIP: exception[%s]\n", e.what());
  }
PYREPR(stdout, lst[1]);
PYREPR(stdout, lst);

PYREPR(stdout, tpl);
PYREPR(stdout, tpl[1]);
  try{
    tpl[1] = PYLNG(-99); // SKIP: exception - not allowed to change tuple item
  }catch(const std::exception &e){
    fprintf(stderr, "SKIP: exception[%s]\n", e.what());
  }
PYREPR(stdout, tpl[1]);
PYREPR(stdout, tpl);

PYREPR(stdout, dct);
PYREPR(stdout, dct["Y"]);
  dct["Y"] = PYDBL(-99);
PYREPR(stdout, dct["Y"]);
PYREPR(stdout, dct);

PYREPR(stdout, b); // kept

  pyob::PyTpl keytpl(std::tie(PYLNG(3), PYLNG(4))); // tuple as an instance
  pyob::PyTpl tmptpl(std::tie(PYLNG(5), PYLNG(6)));
/*
  pyob::PyDct dcttpl;
  dcttpl[PYTPL(PYLNG(1), PYLNG(2))] = PYLNG(11);
*/
  pyob::PyDct dcttpl{ {{PYTPL(PYLNG(1), PYLNG(2)), PYLNG(22)}} };
  dcttpl[PYTPL(PYLNG(3), PYLNG(4))] = PYLNG(44); // tuple as another instance
PYREPR(stdout, dcttpl);
PYREPR(stdout, dcttpl[keytpl]);
  dcttpl[keytpl] = PYDBL(33);
  try{
    pyob::PyBase t = dcttpl[tmptpl]; // SKIP: exception - reference to NULL
  }catch(const std::exception &e){
    fprintf(stderr, "SKIP: exception[%s]\n", e.what());
  }
  dcttpl[tmptpl] = PYLNG(55);
PYREPR(stdout, dcttpl[keytpl]);
PYREPR(stdout, dcttpl);

  pyob::PyDct dctstr({{"a", PYLNG(10)}, {"b", PYLNG(20)}, {"c", PYLNG(30)}});
  dctstr[PYSTR("b")] = PYDBL(99.99);
PYREPR(stdout, dctstr);

  pyob::PyDct dctobj{ {{PYSTR("x"), PYLNG(11)}, {PYSTR("y"), PYLNG(22)}, {PYSTR("z"), PYLNG(33)}} };
  dctobj["y"] = PYDBL(22.22);
PYREPR(stdout, dctobj);
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_mat(void){
try{
  pyob::PyMod np("numpy");
  pyob::PyBase a = (np|"array")(MKTPL(PYTPL(PYTPL(PYLNG(1), PYLNG(2)), PYTPL(PYLNG(3), PYLNG(4)))), {});
PYREPR(stdout, a);
  pyob::PyBase b = (np|"identity")(MKTPL(PYLNG(2)), {});
PYREPR(stdout, b);
  pyob::PyBase c = a * b;
PYREPR(stdout, c);
  pyob::PyBase d = a & b;
PYREPR(stdout, d);
PYREPR(stdout, a & a);

  pyob::PyTpl tpl32(MKTPL(PYLNG(3), PYLNG(2)));
  pyob::PyTpl tpl23(MKTPL(PYLNG(2), PYLNG(3)));
  const char le_b24[32 * 6] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00};
  pyob::PyBin ble24(le_b24, sizeof(le_b24));
  pyob::PyBase i32 = (np|"ndarray")(MKTPL(tpl23, PYSTR("int"), ble24), {});
PYREPR(stdout, i32);
//array([[0, 1, 2], [3, 4, 5]])
  pyob::PyBase f32 = (np|"ndarray")(MKTPL(tpl23, PYSTR("float32"), ble24), {});
PYREPR(stdout, f32);
//array([[  0.00000000e+00,   1.40129846e-45,   2.80259693e-45],
//       [  4.20389539e-45,   5.60519386e-45,   7.00649232e-45]], dtype=float32)
  const char le_b48[64 * 6] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  pyob::PyBin ble48(le_b48, sizeof(le_b48));
  pyob::PyBase f64 = (np|"ndarray")(MKTPL(tpl23, PYSTR("float"), ble48), {});
PYREPR(stdout, f64);
//array([[  0.00000000e+000,   4.94065646e-324,   9.88131292e-324],
//       [  1.48219694e-323,   1.97626258e-323,   2.47032823e-323]])
  pyob::PyTpl tpl6(MKTPL(PYLNG(6)));
  pyob::PyBase f_6 = (np|"ndarray")(MKTPL(tpl6, PYSTR("float"), ble48), {});
PYREPR(stdout, f_6);
//array([  0.00000000e+000,   4.94065646e-324,   9.88131292e-324,
//         1.48219694e-323,   1.97626258e-323,   2.47032823e-323])

  std::vector<double> vec{0., 1., 2., 3., 4., 5.}; // continuity
  pyob::PyBin bvec((const char *)&vec[0], vec.size() * sizeof(vec[0]));
PYREPR(stdout, bvec);
  pyob::PyBase d623 = (np|"ndarray")(MKTPL(tpl23, PYSTR("float64"), bvec), {});
PYREPR(stdout, d623);
  pyob::PyBase d632 = (np|"ndarray")(MKTPL(tpl32, PYSTR("float64"), bvec), {});
PYREPR(stdout, d632);
PYREPR(stdout, d632|"T");

  std::array< std::array<int, 2>, 3 > mat{ {{0, 1}, {2, 3}, {4, 5}} }; // fixed
  for(int maj = 0; maj < 3; ++maj){
    for(int mai = 0; mai < 2; ++mai){
      fprintf(stdout, " %08X", mat[maj][mai]);
    }
    fprintf(stdout, "\n");
  }
  pyob::PyBin bmat((const char *)&mat[0][0],
    mat.size() * mat[0].size() * sizeof(mat[0][0])); // continuity
PYREPR(stdout, bmat);
  pyob::PyBase i632 = (np|"ndarray")(MKTPL(tpl32, PYSTR("int"), bmat), {});
PYREPR(stdout, i632);
PYREPR(stdout, i632|"T");

  int mbuf[] = {0, 1, 2, 3, 4, 5}; // continuity
  std::vector< const int * > mv{&mbuf[0], &mbuf[2], &mbuf[4]}; // fake
  for(int mvj = 0; mvj < 3; ++mvj){
    for(int mvi = 0; mvi < 2; ++mvi){
      fprintf(stdout, " %08X", mv[mvj][mvi]);
    }
    fprintf(stdout, "\n");
  }
  pyob::PyBin bmv((const char *)&mv[0][0], sizeof(mbuf)); // continuity
PYREPR(stdout, bmv);
  pyob::PyBase m623 = (np|"ndarray")(MKTPL(tpl23, PYSTR("int"), bmv), {});
PYREPR(stdout, m623);
PYREPR(stdout, m623|"T");
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_pyplot(const char *title){
try{
  pyob::PyMod np("numpy");
  fprintf(stdout, "%20.17lf\n", double(np|"pi")); // 3.14159265358979323846...
  fprintf(stdout, "%20.17lf\n", double(np|"e")); // 2.71828182845904523536...
  pyob::PyBase x = (np|"arange")(std::tie(PYLNG(-99), PYLNG(99), PYDBL(.2)), {});
  pyob::PyBase y = (np|"arange")(std::tie(PYLNG(-30), PYLNG(30), PYDBL(.1)), {});
  pyob::PyBase m = (np|"meshgrid")(std::tie(x, y), {});
  pyob::PyBase zz = (np|"cos")(MKTPL(m[0]), {}) * (np|"sin")(MKTPL(m[1]), {});
//  pyob::PyBase zz = (np|"sqrt")(MKTPL((m[0] ^ 2) + (m[1] ^ 2)), {});
//PYREPR(stdout, zz);

  pyob::PyMod matplotlib("matplotlib");
  pyob::PyMod pyplot("matplotlib.pyplot");
  pyob::PyMod pylab("pylab");

  pyob::PyBase ax = (pyplot|"contour")(MKTPL(m[0] * 3.5, m[1] / 5.0, zz), {});

if(0){ // (char **) selffree=false
  pyob::PyBase res = pyplot.c("title", 1, false, "(s)", title);
}

if(0){ // selffree=false
  PyObject *args = PyTuple_New(1);
  PyTuple_SetItem(args, 0, pyob::PyStr(title, false).o());
  PyObject *kwargs = PyDict_New();
  PyObject *res = pyplot.call("title", args, kwargs);
  Py_DECREF(args);
  Py_DECREF(kwargs);
  if(res) Py_DECREF(res);
}

if(0){ // selffree=false
  pyob::PyTpl args(1);
  args.setitem(0, (pyob::PyBase &)pyob::PyStr(title, false));
  pyob::PyDct kwargs;
  pyob::PyBase res = pyplot.c("title", args, kwargs);
}

if(0){ // (char **)
  pyob::PyTpl &args = (pyob::PyTpl &)pyob::PyBase(true, "(s)", title);
  pyob::PyDct &kwargs = (pyob::PyDct &)pyob::PyBase(true, "{}");
  pyob::PyBase res = pyplot.c("title", args, kwargs);
}

if(0){ // (char **)
  pyob::PyTpl &args = (pyob::PyTpl &)pyob::PyBase(true, "(s)", title);
  pyob::PyBase res = pyplot.c("title", args);
}

if(0){ // (char **)
  pyob::PyBase res = pyplot.c("title",
    (pyob::PyTpl &)pyob::PyBase(true, "(s)", title));
}

if(0){ // selffree=false
  pyob::PyStr t(title, false);
  pyob::PyBase res = pyplot.c("title", std::tie(t), {});
}

if(0){ // selffree=false
  pyob::PyStr t(title);
  pyob::PyBase res = pyplot.c("title", std::tie(t), {});
}

if(0){ // selffree=false
  pyob::PyBase t = PYSTR(title, false);
  pyob::PyBase res = pyplot.c("title", std::tie(t), {});
}

if(0){ // selffree=false
  pyob::PyBase &t = PYSTR(title, false);
  pyob::PyBase res = pyplot.c("title", std::tie(t), {});
}

if(0){ // OK
  pyob::PyBase res = pyplot.c("title", std::tie(PYSTR(title)), {});
}

if(1){ // OK
  pyob::PyBase res = (pyplot|"title")(std::tie(PYSTR(title)), {});
}
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_wx(const char *title){
try{
  pyob::PyMod wx("wx");
  pyob::PyBase app = (wx|"App")();
/*
  pyob::PyBase frm = wx.c("Frame",
    (pyob::PyTpl &)pyob::PyBase(true, "si", NULL, -1),
    (pyob::PyDct &)pyob::PyBase(true, "{sssOsO}",
      "title", title,
      "size", pyob::PyBase(true, "ii", 640, 480).o(),
      "pos", pyob::PyBase(true, "ii", 320, 240).o()));
*/
/*
  pyob::PyBase frm = wx.c("Frame",
    (pyob::PyDct &)pyob::PyBase(true, "{sssisssOsO}",
      "parent", NULL,
      "id", -1,
      "title", title,
      "size", pyob::PyBase(true, "ii", 640, 480).o(),
      "pos", pyob::PyBase(true, "ii", 320, 240).o()));
*/
/*
  pyob::PyBase frm = wx.c("Frame", 1, true, "si", NULL, -1);
*/
/*
  pyob::PyBase frm = wx.c("Frame", 1, true, "Oi",
    pyob::PyBase(true, "").o(), -1);
*/
/*
  pyob::PyBase frm = wx.c("Frame", 2, true, "{sssisssOsO}",
      "parent", NULL,
      "id", -1,
      "title", title,
      "size", pyob::PyBase(true, "ii", 640, 480).o(),
      "pos", pyob::PyBase(true, "ii", 320, 240).o());
*/
  pyob::PyBase frm = (wx|"Frame")(std::tie(PYNONE, PYLNG(-1)), {
      {"title", PYSTR(title)},
      {"size", PYBV("ii", 640, 480)}, {"pos", PYBV("ii", 320, 240)}});

  pyob::PyMod _builtins("builtins");
fprintf(stderr, "_builtins: %08X\n", _builtins.o());

/*
  PyRun_SimpleString("AZ = 'zzz'");
  pyob::PyBase _g = (_builtins|"globals")();
pyob::PyBase::p("_g: ", _g.len()); // !!!! not reach (exception)
fwprintf(stderr, L"[%s]\n", _g.ascii());
  pyob::PyBase _z = ((_builtins|"globals")()|"get")(std::tie(PYSTR("AZ")), {});
pyob::PyBase::p("_z: ", _z.len()); // !!!! not reach (exception)
fwprintf(stderr, L"[%s]\n", _z.ascii());
*/

  pyob::PyMod _main("__main__");
  pyob::PyBase _globals = pyob::PyBase(PyModule_GetDict(_main.o()));
  pyob::PyBase _locals = pyob::PyDct();
  const char *TESTCODE = "open('c:/tmp/_dummy_text.txt', 'wb').write(b'ABCD')";
  pyob::PyBase _b = (_builtins|"eval")(std::tie(PYSTR(TESTCODE), _globals, _locals), {});
/* OK
  pyob::PyBase _co(Py_CompileString(TESTCODE, "dummy", Py_eval_input));
  pyob::PyBase _b(PyEval_EvalCode(_co.o(), _globals.o(), _locals.o()));
*/
// pyob::PyBase::p("_b: ", _b.len()); // not str
fwprintf(stderr, L"[%06d]\n", (long)_b); // 000004 OK

  pyob::PyBase _h = (_builtins|"bytes"|"fromhex")(std::tie(PYSTR("55AA")), {});
pyob::PyBase::p("_h: ", _h.len()); // 0x0002
fwprintf(stderr, L"[%s]\n", _h.ascii()); // OK
fwprintf(stderr, L"[%s]\n", (wchar_t *)_h); // OK
fprintf(stderr, "[%s]\n", (char *)_h); // 55 AA OK
  pyob::PyBase::s((wchar_t *)_h); // OK

  pyob::PyBase _u = (_builtins|"bytes"|"fromhex")(std::tie(PYSTR("e9a1b1")), {});
fwprintf(stderr, L"[%s]\n", _u.repr()); // OK
fwprintf(stderr, L"[%s]\n", _u.ascii()); // OK
fwprintf(stderr, L"[%s]\n", (wchar_t *)_u); // OK (force bytes as str)
fprintf(stderr, "[%s]\n", (char *)_u); // OK
  pyob::PyBase _w = (_u|"decode")();
fwprintf(stderr, L"[%s]\n", _w.repr()); // ['?'] OK
fwprintf(stderr, L"[%s]\n", _w.ascii()); // ['\u9871'] OK
fwprintf(stderr, L"[%s]\n", (wchar_t *)_w); // [?] L.E. 71 98 OK
//fprintf(stderr, "[%s]\n", (char *)_w); // BAD (force str as bytes) no '\0' ?

  pyob::PyBin __binicon(NULL, 0);
pyob::PyBase::p("__binicon", __binicon.len());
fwprintf(stderr, L"[%s]\n", __binicon.ascii()); fflush(stderr);
  const char *TESTDATA = "./binicon.bin";
  FILE *fp = NULL;
  errno_t err = fopen_s(&fp, TESTDATA, "rb");
  if(err || !fp) fprintf(stderr, "can not open [%s]\n", TESTDATA);
  else{
    char buf[1024]; // *** be careful ***
    size_t rc = fread(buf, 1, sizeof(buf), fp);
fprintf(stdout, "read: %06d [%s]\n", rc, TESTDATA); fflush(stdout);
    fclose(fp);
    __binicon = pyob::PyBin(buf, rc);
  }
pyob::PyBase::p("__binicon", __binicon.len());
fwprintf(stderr, L"[%s]\n", __binicon.ascii()); fflush(stderr);

  const char *ascicon =
    "789cc592310e8330100497e006450afe41a8229e41e187f184b4f9457a1a3f27\x0A"
    "657a8acbad6d029130a241597b59e6245b823ba0c0094d63350d5e27e006c0da\x0A"
    "c86d015c4aa0d55aa3eed4ac6fab0f1b7d1f834b5fbcf79aac48d81089c1c512\x0A"
    "1f07ab7a50f73c4be2f3403df32cbb395cbc60e35439d6d346f2ccab220fdbbc\x0A"
    "f8e6c0aedbcf6e9b8f94ac08e5648f55e7eefaba9455af88736e1167fd1f735e\x0A"
    "a571a999dacf902313212b9d966e649f6bd70d29e389d473f3d3fbecbf91e882\x0A"
    "ee672ff5a6afb3a733f407b4ecff08\x0A";
  pyob::PyStr ascdat(ascicon);
pyob::PyBase::p("ascdat", ascdat.len()); // 0x019e + LF x 7
fwprintf(stderr, L"%s\n", ascdat.ascii()); fflush(stderr);

  pyob::PyBase hex = (ascdat|"replace")(std::tie(PYSTR("\n"), PYSTR("")), {});
pyob::PyBase::p("hex", hex.len());
fwprintf(stderr, L"%s\n", hex.ascii()); fflush(stderr);

  pyob::PyBase _binicon = (_builtins|"bytes"|"fromhex")(std::tie(hex), {});
pyob::PyBase::p("_binicon", _binicon.len());
fwprintf(stderr, L"[%s]\n", _binicon.ascii()); fflush(stderr);

  pyob::PyMod binascii("binascii");
  pyob::PyBase binicon = (binascii|"a2b_hex")(std::tie(hex), {});
pyob::PyBase::p("binicon", binicon.len());
fwprintf(stderr, L"[%s]\n", binicon.ascii()); fflush(stderr);

  pyob::PyMod zlib("zlib");
  pyob::PyBase bindat = (zlib|"decompress")(std::tie(binicon), {});
pyob::PyBase::p("bindat", bindat.len()); // 0x0436
  pyob::PyMod pyio("io");
  pyob::PyBase strm = (pyio|"BytesIO")(std::tie(bindat), {});
  pyob::PyBase im = (wx|"Image")(std::tie(strm), {});
  pyob::PyBase bm = (wx|"Bitmap")(std::tie(im), {});
  pyob::PyBase icon = (wx|"Icon")();
  (icon|"CopyFromBitmap")(std::tie(bm), {});
  (frm|"SetIcon")(std::tie(icon), {});

  pyob::PyBase r0 = (frm|"Show")();
/*
  pyob::PyBase r1 = app.c("SetTopWindow",
    (pyob::PyTpl &)pyob::PyBase(true, "O", frm.o()));
*/
/*
  pyob::PyBase r1 = app.c("SetTopWindow", 1, true, "O", frm.o());
*/
  pyob::PyBase r1 = (app|"SetTopWindow")(std::tie(frm), {});
  pyob::PyBase r2 = (app|"MainLoop")();
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}

void test_mm(void){
try{
  pyob::PyMod sys("sys");
  pyob::PyBase a = (sys|"path"|"append")(std::tie(PYSTR(".")), {});

  pyob::PyMod mm("mm"); // mm.py must be at '.'
  pyob::PyBase mc = (mm|"mc")(std::tie(PYLNG(1234)), {});
  pyob::PyBase r = (mc|"pi")(std::tie(mm, mc), {{"a", mm}, {"b", mc}});
pyob::PyBase::p("r: ", r.len());
fwprintf(stderr, L"[%s]\n", r.ascii());
  pyob::PyBase o = (mc|"pi")(
    std::tie(PYSTR("a-z"), PYLNG(-1), PYLST(PYLNG(-3)), PYTPL(PYLNG(-5))),
    {{"a", PYSTR("A-Z")}, {"b", PYLNG(-2)}, {"F", PYDBL(1.5/15.0)},
      {"I", PYDCT({{"-", PYLNG(-9)}, {"+", PYLNG(9)}})},
      {"c", PYLST(PYLNG(-4), PYSTR("xy"))}, {"d", PYTPL(PYLNG(-6), PYNONE)}});
pyob::PyBase::p("o: ", o.len());
fwprintf(stderr, L"[%s]\n", o.ascii());
}catch(const std::exception &e){
  fprintf(stderr, "exception[%s]\n", e.what());
}
}
