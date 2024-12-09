# An Android Applaketion

* **Category**: rev
* **Solved by** 61 teams
* **Final score**: 170
* **Provided files**: [AndroidApplaketion.apk](AndroidApplaketion.apk)

just an Android reg challenge


## Write-up

**Note**: This is NOT an original write-up. I cleaned it up from [sup3rshy's](https://github.com/sup3rshy/CTF/blob/main/lakectf/An%20Android%20Applaketion/solve.py).

### Reproducing

1. Unpack `AndroidApplaketion.apk`.
2. Open `lib/x86_64/libohgreat.so` with ida64.
3. Save `libohgreat.so.c` using File -> Produce file -> Create C file.
4. Run `solve.py`.

### Explanation

The library has 600 small functions called `Java_com_lake_ctf_MainActivity_EPFL<SOME_RANDOM_HASH>`, all possessing the same structure. Let's take a look at one of them:

```c
bool __fastcall Java_com_lake_ctf_MainActivity_EPFL9d7f2599584878923238b042f43f564fe1dc2a575866f351063d2070dc6f793a(
        __int64 a1,
        __int64 a2,
        __int64 a3)
{
  _BYTE *v3; // rax

  v3 = (_BYTE *)(*(__int64 (__fastcall **)(__int64, __int64, _QWORD))(*(_QWORD *)a1 + 1352LL))(a1, a3, 0LL);
  return (v3[18] ^ (unsigned __int8)(v3[15] ^ v3[55])) == 102;
}
```

First, the function calls `(*(_QWORD *)a1 + 1352LL))`, which is a pointer to the `const char * GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy)` function inside the `JNIEnv` struct. Please [take a look here](https://github.com/maaaaz/jnianalyzer) for more details about JNI.

Thus `v3` is a `const char *` pointing to the flag chars encoded as UTF-8.

The small function proceeds to check a boolean condition involving some of the string chars.

The [solve.py](solve.py) script reads all of these small functions and converts their boolean conditions to Python expressions.

But not all of the 600 small functions are called at all. The scripts proceeds to check which functions are referenced by the `JNI_OnLoad` function (only 80 of them).

For every function which is actually called, the script adds the corresponding expression evaluated as a z3 equation to a z3 SAT solver.

Finally, SAT is solved to get a flag satisfying all equations: `EPFL{R3g1st3r_R3g1st3r_1n_L1b4rt.s0_wh3r3_w1ll_my_JN1_C4ll_g0?}`

### Prank

Please note there is a prank in this challenge. If instead of adding the functions referenced by `JNI_OnLoad`, we simply looked at the name of the functions called by [MainActivity.java](MainActivity.java) (decompiled from the apk using [bytecodeviewer](https://github.com/Konloch/bytecode-viewer)), we would end up adding the wrong functions to the system of equations and would get **unsat**. This happens because `JNI_OnLoad` calls `JNIEnv->RegisterNatives` to rename the C functions when exposing them to the Java code.
