## Intro

   Pill is an open source interpreter for the Cadence Skill language. Its purpose is to run PCell generator codes used in VLSI. Pill is written in Python and compiles the source into Python bytecode where it is then executed alongside regular python functions at similar speed to "native" python codes. Geometry functions are called through the Klayout API to yield usable gds2.
  
  The process of importing PDK codes and then validating the results is tedious. At this time, Pill is more of a tool to help you create a working Klayout-based PDK than an out-of-the-box solution to import. 
  
## Requirements
- Python 3.7+
- Klayout built with Python 3.7+
- Parsimonious, eg. pip3 install parsimonious
- Python bytecode package, eg. pip3 install bytecode
- G++ for CDB/Virtuso binary tools
   
## CDB Examples
1. Run 'git submbodule update --init' to download the example PDK
2. cd into ./binary
3. Run ./build.sh
4. Run ./proc_examples.sh
5. From the root directory, run ./entry.py
6. This will takes some time to compile all of the cells, future runs will be faster. 
7. Open foo.gds in klayout

## Getting started with PCELLS
   First you must extract data from the PDK and make appropriate edits to entry.py. Entry.py is a template file for cell generation.
   1.  Find your layermap file. Pill knows how to read files with '#' comments and 4 element tuples describing each layer. Such as:
```
 ref		drawing		0	0`
```
   Copy this file into the pill directory
   In entry.py, change `layermap_file` to the filename.
   
   2.  In virtuoso, run:

```
    dbOpenBag(ddGetObj("@lib","@cell"))~>prop~>??
```

Copy and paste the output into yourcell_props.il then edit `props_file` in entry.py to this filename

   3. In virtuoso, run: 
   ```
dbDumpPcDefinePcell(dbOpenCellViewByType("@lib" "pch" "layout" "maskLayout") "/tmp/pch.il")
```
4. Open the resulting file. There should be 3 primary sections, the first being a description of the cell. The second is a list of triplet parameters, and the third is code. The triplets needs to be copied into the `defaults =` section of the entry.py. Typically the code section wraps a single function call. We will make the target of that call our `func=` in entry.py.
5. Extract code. Use `pp(functionName)` in Virtuoso to pretty-print the skill of interest. Create a .il file with the function. In practice this function will call many other PDK functions that must also be exported. It is best to not have functions that self-reference in the same file, so you will end up with several files that must be loaded in a specific order. These will be listed in the `codes=[]`.
6. Execute entry.py and the pcell should be generated
7. In your props.il there will be at least one function near the text "callback", this function should also be exported to allow the default parameters to be modified. 

## Notes

The compiler is fairly slow. The compiled bytecode objects are cached, so subsequent runs will be faster. If codegen changes, you will need to force a recompile (not necessary if only runtime functions are modified). To force a recompile, bump the iversion. Modified skill files are automatically recompiled. 

The Skill language is difficult to parse, and the meaning of statements changes drastically depending on if an identifier is a function. In many cases a parse error will be caused by undelcared functions (which are incorrectly assumed to be variables). The parsing errors give approximate line locations which often correlate to an undeclared function, typically another PDK function you need to export, or a missing runtime function. 

Sometimes functions needs to be defined, but don't need to exist.  For example "xxxx_customer_Callback" style functions. To overcome these issues, before calling cload(), add a line like `interp.skill.procedures['xxxx_customer_Callback'] = None`

## Python interop
Skill calls python code through the interp.skill.procedures dictionary, which is largely populated in runtime.py. Types are largely native or can be coerced into native types. Very little needs to be done in order to marshal things in/out of the python world. In some rare cases a piece of code will call ~> on something that is actually a list, in which case returned lists should be instances of tools.SkillDeref instead. 

## Binary contexts
Many people do not have the SkillDev license required to pretty-print skill code from within Virtuoso. If your PDK is distributed in binary form, the binary subdirectory contains a program that can dump 64bit context files. This is a work in progress, but generates 99% working code. 

## Encrypted Skill
The decrypt tool in the binary directory is able to extract usable skill code from .ile files

## CDB File format
A beta tool is available for extracting the contents of CDB format files. Currently it seems to extract usable Skill and static geometry, but pcell extraction is somewhat manual as far as extracting code and props, and then importing into Pill. Currently only loads files written from big-endian 32-bit machines because I don't have any other CDB's to test on. 
### Loading static CDB files
1. cd binary
2. ./build.sh
3. ./xx /path/to/pdk/library/cell_name/LAYOUT/LAYOUT.CDB      //It is important that the destination path is formatted correctly as the cell name is extracted from the subdirectory
4. The output will be written to output/cell_name.il   //Viewing this file will show any instantiated cells that may also need to be extracted
5. Edit static_cells[] in entry.py to load the cells. 
6. Change interp.layout("cell_name") in entry.py to correct cell
7. Run ./entry.py and the output will be generated into "foo.gds"

