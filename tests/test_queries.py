from pathlib import Path
import csv,io,math,os,random,re,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
PROGRAM=os.environ.get('PROGRAM',str(ROOT/'map2'))
HEADER=['Census year','Block ID','Property ID','Base property ID','CLUE small area','Trading name','Industry (ANZSIC4) code','Industry (ANZSIC4) description','x coordinate','y coordinate','Location']
def data(points):
    out=io.StringIO();writer=csv.writer(out);writer.writerow(HEADER)
    for i,(x,y) in enumerate(points,1):writer.writerow([2018,i,i,i,'Area','Name, "quoted"',1234,'Description',x,y,f'({y}, {x})'])
    return out.getvalue()
class QueryTests(unittest.TestCase):
    def query(self,text,queries,valid=True):
        with tempfile.TemporaryDirectory(dir=ROOT/'.build') as d:
            inp=Path(d)/'input.csv';out=Path(d)/'output.txt';inp.write_text(text)
            r=subprocess.run([PROGRAM,str(inp),str(out)],input=queries,capture_output=True,text=True,timeout=15)
            self.assertNotIn('AddressSanitizer',r.stderr);self.assertNotIn('runtime error:',r.stderr)
            self.assertEqual(r.returncode==0,valid,r.stderr)
            return out.read_text() if out.exists() else ''
    def test_duplicates_radius_and_quoted_fields(self):
        out=self.query(data([(0,0),(0,0),(1,1)]),'0 0 0\n0 0\n10 10 1\n')
        self.assertEqual(out.count('Property ID: 1 ||'),2)
        self.assertIn('Name, "quoted"',out);self.assertIn('10 10 1 --> NOTFOUND',out)
    def test_empty_tree_and_invalid_inputs(self):
        self.assertIn('NOTFOUND',self.query(data([]),'0 0\n'))
        for text in ['', 'bad header\n',','.join(HEADER)+'\n2018,1,2\n', data([(0,0)]).replace('"(0, 0)"','"(nan, 0)"')]:
            self.query(text,'0 0\n',False)
        for q in ['bad\n','0\n','0 0 -1\n','0 0 1 extra\n','nan 0\n','A'*10000+'\n']:
            self.query(data([(0,0)]),q,False)
    def test_exhaustive_distance_oracle(self):
        rng=random.Random(2026);points=[(rng.randrange(-20,21),rng.randrange(-20,21)) for _ in range(40)]
        queries=[];expected={}
        for i in range(80):
            x=rng.uniform(-25,25);y=rng.uniform(-25,25);radius=rng.randrange(1,10)
            key=f'{x:.9f} {y:.9f} {radius}';queries.append(key)
            expected[key]={j for j,(a,b) in enumerate(points,1) if math.hypot(a-float(key.split()[0]),b-float(key.split()[1]))<=radius}
        out=self.query(data(points),'\n'.join(queries)+'\n');actual={k:set() for k in expected}
        for line in out.splitlines():
            if ' --> ' not in line:continue
            key=line.split(' --> ')[0]
            m=re.search(r'\|\| Property ID: (\d+) \|\|',line)
            if m:actual[key].add(int(m[1]))
        self.assertEqual(actual,expected)
        for x,y in [(0.1,0.2),(20.1,-20.2),(-20.1,20.2)]:
            out=self.query(data(points),f'{x} {y}\n')
            ids=set(map(int,re.findall(r'\|\| Property ID: (\d+) \|\|',out)))
            best=min(math.hypot(a-x,b-y) for a,b in points)
            self.assertTrue(ids)
            self.assertTrue(all(math.isclose(math.hypot(points[j-1][0]-x,points[j-1][1]-y),best) for j in ids))
    def test_large_degenerate_tree_and_real_dataset(self):
        self.assertIn('Property ID: 2000 ||',self.query(data([(i,0) for i in range(2000)]),'1999 0 0\n'))
        real=(ROOT/'datafile/datafile_original.csv').read_text()
        self.assertIn('Trading Name:',self.query(real,'144.967058 -37.817313 0.0005\n'))
    def test_bad_args_and_input_output_alias(self):
        r=subprocess.run([PROGRAM],capture_output=True,timeout=5);self.assertNotEqual(r.returncode,0)
        with tempfile.TemporaryDirectory(dir=ROOT/'.build') as d:
            f=Path(d)/'data.csv';original=data([(0,0)]);f.write_text(original)
            r=subprocess.run([PROGRAM,str(f),str(f)],input='0 0\n',capture_output=True,text=True,timeout=5)
            self.assertNotEqual(r.returncode,0);self.assertEqual(f.read_bytes(),original.encode())
if __name__=='__main__':unittest.main()
