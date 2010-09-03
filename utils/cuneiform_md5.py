#!/usr/bin/env python                                                                                                                                         
                                                                                                                                                              
import hashlib                                                                                                                                                
                                                                                                                                                              
FILES = (                                                                                                                                                     
    'alphabets',                                                                                                                                              
    'ccom',                                                                                                                                                   
    'ced',                                                                                                                                                    
    'cfio',                                                                                                                                                   
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
    'rmarker',                                                                                                                                                
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
                                                                                                                                                              
def make_lib_filename(name):                                                                                                                                  
    return '@CMAKE_BINARY_DIR@/src/%s/libcuneiform-%s.so' % (name, name)                                                                                                         
                                                                                                                                                              
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
    values.update(md5sum("@CMAKE_BINARY_DIR@/src/puma/libcuneiform.so"))                                                                                      
    values.update(md5sum("@CMAKE_BINARY_DIR@/src/rshelllines/libcuneiform-rsl.so"))                                                                                          
    return values

if __name__ == '__main__':                                                                                                                                    
    print generate() 
