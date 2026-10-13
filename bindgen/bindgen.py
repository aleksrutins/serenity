#!/usr/bin/env python3
"""Generate sol2 Lua bindings from a small C++-oriented IDL."""
from __future__ import annotations
import argparse
import pathlib
import re
import sys

TOKEN = re.compile(r'\s+|//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|->|::|>=|<=|==|!=|&&|\*|[A-Za-z_][A-Za-z_0-9]*|[0-9]+|[^\s]', re.S)

class ParseError(Exception): pass

class Parser:
    def __init__(self, source: str):
        self.ts = [m.group(0) for m in TOKEN.finditer(source) if not m.group(0).isspace() and not m.group(0).startswith('//') and not m.group(0).startswith('/*')]
        self.i = 0
    def peek(self): return self.ts[self.i] if self.i < len(self.ts) else ''
    def take(self, expected=None):
        t = self.peek()
        if not t: raise ParseError('unexpected end of input')
        if expected is not None and t != expected: raise ParseError(f'expected {expected!r}, got {t!r}')
        self.i += 1
        return t
    def until(self, stops):
        out=[]; depth=0
        while self.peek():
            t=self.peek()
            if depth == 0 and t in stops: break
            t=self.take(); out.append(t)
            if t in ('(', '<', '[', '{'): depth += 1
            elif t in (')', '>', ']', '}'): depth -= 1
        text=''
        for token in out:
            if text and re.match(r'^[A-Za-z_0-9]', token) and re.search(r'[A-Za-z_0-9]$', text):
                text += ' '
            text += token
        return text.strip()
    def parse(self):
        items=[]
        while self.peek():
            if self.peek() == ';': self.take(); continue
            items.append(self.declaration())
        return items
    def declaration(self):
        if self.peek() == 'include':
            self.take()
            header=self.until({';'})
            self.take(';')
            return ('include', header)
        if self.peek() == 'using':
            self.take(); self.take('namespace'); name=self.until({';'}); self.take(';')
            return ('using', name)
        opaque=False
        if self.peek() == 'opaque': self.take(); opaque=True
        kind=self.take()
        if kind == 'namespace':
            name=self.take(); self.take('{'); body=self.block('}'); self.take('}')
            return ('namespace',name,body)
        if kind == 'global':
            self.take('{'); body=self.block('}'); self.take('}')
            return ('global',body)
        if kind != 'class': raise ParseError(f'expected namespace or class, got {kind!r}')
        name=self.take(); cpp=name
        if self.peek() == '(':
            self.take('('); cpp=self.until({')'}); self.take(')')
        bases=[]
        if self.peek() == ':':
            self.take(':')
            while True:
                bases.append(self.until({',','{'}))
                if self.peek() != ',': break
                self.take(',')
        ref=cpp
        if opaque:
            self.take(';'); return ('opaque',name,cpp,bases)
        self.take('{'); body=self.block('}'); self.take('}')
        if self.peek() == ';': self.take()
        return ('class',name,cpp,bases,ref,body)
    def block(self, end):
        items=[]
        while self.peek() and self.peek()!=end:
            if self.peek()==';': self.take(); continue
            items.append(self.member())
        if not self.peek(): raise ParseError(f'missing {end!r}')
        return items
    def member(self):
        if self.peek() == 'factory':
            self.take(); name=self.take(); args=self.arguments(); alias=name
            if self.peek() == '->': self.take(); alias=self.until({';'})
            self.take(';'); return ('factory',name,args,alias)
        if self.peek() == 'new':
            self.take(); args=self.arguments(); alias='new'
            if self.peek() == '->': self.take(); alias=self.until({';'})
            self.take(';'); return ('new',args,alias)
        if self.peek() == 'static':
            self.take(); name=self.take(); self.take('->'); expr=self.until({';'}); self.take(';')
            return ('static',name,expr)
        typ=self.until({'(' ,';','->'})
        if self.peek() == '(':
            match=re.match(r'^(.*?)([A-Za-z_][A-Za-z_0-9]*)$', typ)
            if not match: raise ParseError(f'invalid function declaration {typ!r}')
            ret,name=match.group(1).strip(),match.group(2)
            args=self.arguments(); alias=name
            if self.peek()=='->': self.take(); alias=self.until({';'})
            self.take(';'); return ('method',ret,name,args,alias)
        name=typ
        alias=name
        if self.peek()=='->': self.take(); alias=self.until({';'})
        self.take(';'); return ('value',name,alias)
    def arguments(self):
        self.take('('); args=[]
        if self.peek()!=')':
            while True:
                args.append(self.until({',',')'}))
                if self.peek()!=',': break
                self.take(',')
        self.take(')'); return args

def cpp_args(args): return ', '.join(args)
def quoted(s): return '"'+s.replace('\\','\\\\').replace('"','\\"')+'"'
def emit(items):
    out=[]
    for item in items:
        k=item[0]
        if k=='include': out.append(f'#include {item[1]}')
        elif k=='using': out.append(f'using namespace {item[1]};')
        elif k=='namespace':
            _,name,body=item
            funcs=[x for x in body if x[0] in ('method','value')]
            out.append(f'lua[{quoted(name)}] = lua.create_table();')
            grouped={}
            for f in funcs:
                if f[0]=='method':
                    _,ret,fn,args,alias=f
                    cpp=f'static_cast<{ret}(*)({cpp_args(args)})>({alias})'
                else: _,fn,alias=f; cpp=alias
                grouped.setdefault(fn,[]).append(cpp)
            for fn,overloads in grouped.items():
                value=overloads[0] if len(overloads)==1 else 'overload('+', '.join(overloads)+')'
                out.append(f'lua[{quoted(name)}][{quoted(fn)}] = {value};')
        elif k=='global':
            grouped={}
            for f in item[1]:
                if f[0]=='value': _,name,expr=f
                elif f[0]=='method':
                    _,ret,name,args,alias=f
                    expr=f'static_cast<{ret}(*)({cpp_args(args)})>({alias})'
                else: raise ParseError(f'{f[0]} is not valid in a global block')
                grouped.setdefault(name,[]).append(expr)
            for name,values in grouped.items():
                value=values[0] if len(values)==1 else 'overload('+', '.join(values)+')'
                out.append(f'lua[{quoted(name)}] = {value};')
        elif k=='opaque':
            _,name,cpp,bases=item
            out.append(f'lua.new_usertype<{cpp}>({quoted(name)});')
        elif k=='class':
            _,name,cpp,bases,ref,body=item
            args=[]
            constructors=[x for x in body if x[0]=='new']
            if constructors:
                lambdas=[]
                for ctor in constructors:
                    _,types,_=ctor; names=[f'arg{j}' for j in range(len(types))]
                    decl=', '.join(f'{t} {v}' for t,v in zip(types,names))
                    lambdas.append(f'[]({decl}){{return new {ref}({", ".join(names)});}}')
                factory='factories('+lambdas[0]+')' if len(lambdas)==1 else 'overload('+', '.join(lambdas)+')'
                args.append(f'{quoted("new")}, {factory}')
            methods={}
            for m in body:
                if m[0]=='method':
                    _,ret,fn,types,alias=m
                    cast=f'static_cast<{ret} ({ref}::*)({cpp_args(types)})>(&{ref}::{alias})'
                    methods.setdefault(fn,[]).append(cast)
                elif m[0]=='factory':
                    _,fn,types,alias=m
                    args.append(f'{quoted(fn)}, factories(&{ref}::{alias})')
                elif m[0]=='static':
                    continue
                elif m[0]=='value':
                    _,fn,expr=m; args.append(f'{quoted(fn)}, {expr}')
            for fn,overloads in methods.items():
                value=overloads[0] if len(overloads)==1 else 'overload('+', '.join(overloads)+')'
                args.append(f'{quoted(fn)}, {value}')
            if bases:
                baseexpr=', '.join(bases)
                args.append(f'sol::base_classes, sol::bases<{baseexpr}>()')
            out.append(f'lua.new_usertype<{ref}>({quoted(name)}' + (',\n    '+',\n    '.join(args) if args else '') + ');')
            for m in body:
                if m[0]=='static':
                    _,fn,expr=m
                    out.append(f'lua[{quoted(name)}][{quoted(fn)}] = {expr};')
    return '\n\n'.join(out)+'\n'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('input', help='IDL source file')
    ap.add_argument('-o','--output', required=True, help='generated C++ header path')
    ns=ap.parse_args()
    try: generated=emit(Parser(pathlib.Path(ns.input).read_text()).parse())
    except (OSError,ParseError) as e:
        print(f'bindgen: {e}',file=sys.stderr); return 1
    dst=pathlib.Path(ns.output); dst.parent.mkdir(parents=True,exist_ok=True); dst.write_text('// Generated by serenity-bindgen. Do not edit.\n'+generated)
    return 0
if __name__=='__main__': raise SystemExit(main())
