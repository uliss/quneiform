#!@PYTHON_EXECUTABLE@
                                                                                                                                                              
import hashlib                                                                                                 
import platform

LIB_EXT = 'so'
if platform.system() == 'Darwin':
    LIB_EXT = 'dylib'

                                                                                                                        
FILES = (                                                                                                                                                     
    'alphabets',                                                                                                                                              
    'ccom',                                                                                                                                                   
    'ced',                                                                                                                                                    
    'cimage',                                                                                                                                                 
    'cline',                                                                                                                                                  
    'common',                                                                                                                                                 
    'compat',                                                                                                                                                 
    'cpage',                                                                                                                                                  
    'cpu',                                                                                                                                                    
    'cstr',                                                                                                                                                   
    'ctb',                                                                                                                                                    
    'dif',                                                                                                                                                    
    'evn',                                                                                                                                                    
    'exc',                                                                                                                                                    
    'export',                                                                                                                                                 
    'fon',                                                                                                                                                    
    'leo',                                                                                                                                                    
    'lns',                                                                                                                                                    
    'loc',                                                                                                                                                    
    'mmx',                                                                                                                                                    
    'msk',                                                                                                                                                    
    'pass2',                                                                                                                                                  
#    'puma',                                                                                                                                                  
    'r35',                                                                                                                                                    
    'rbal',                                                                                                                                                   
    'rblock',                                                                                                                                                 
    'rcorrkegl',                                                                                                                                              
    'rcutp',                                                                                                                                                  
    'rdib',                                                                                                                                                   
    'rfrmt',                                                                                                                                                  
    'rimage',                                                                                                                                                 
    'rline',                                                                                                                                                  
    'rling',                                                                                                                                                  
    'markup',                                                                                                                                                
    'rneg',                                                                                                                                                   
    'rpic',                                                                                                                                                   
    'rpstr',                                                                                                                                                  
    'rreccom',                                                                                                                                                
    'rsadd',                                                                                                                                                  
    'rselstr',                                                                                                                                                
#    'rshelllines',                                                                                                                                           
    'rstr',                                                                                                                                                   
    'rstuff',                                                                                                                                                 
    'rverline',                                                                                                                                               
    'smetric',                                                                                                                                                
    'std'                                                                                                                                                     
)                                                                                                                                                             
                                                                                                                                                              
def make_lib_filename(name, name2 = None):                                                                                                                                  
    if name2 is None:
    	return '@CMAKE_BINARY_DIR@/src/%s/libcuneiform-%s.%s' % (name, name, LIB_EXT)                                                                                                         
    else:
    	return '@CMAKE_BINARY_DIR@/src/%s/%s.%s' % (name, name2, LIB_EXT)                                                                                                         
                                                                                                                                                              
def md5sum(filename):                                                                         
    m = hashlib.md5()                                                                                                                                         
    try:                                                                                                                                                      
        f = open(filename, 'r')                                                                                                                               
    except:                                                                                                                                                   
        print "Can't open: %s" % filename                                                                                                                     
        return                                                                                                                                                
                                                                                                                                                              
    m.update(f.read())                                                                                                                                        
    return { m.hexdigest() : filename }
                                                                                                                    
def generate():                                                                                                                                               
    values = {}
    for name in FILES:                                                                                                                                        
        values.update(md5sum(make_lib_filename(name)))

    values.update(md5sum("@CMAKE_BINARY_DIR@/cuneiform"))                                                                                                  
    values.update(md5sum(make_lib_filename("puma", "libcuneiform")))                                                                                     
    values.update(md5sum(make_lib_filename("rshelllines", "libcuneiform-rsl")))                                                                                          
    return values

if __name__ == '__main__':                                                                                                                                    
    print generate() 
