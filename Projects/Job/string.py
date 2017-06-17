# -*- coding:utf-8 -*-
class Same:
    def checkSam(self, stringA, stringB):
        # write code here
        lA = list(stringA); lB = list(stringB)
        dA = {}; dB = {}
        
        for a in lA:
            if dA.has_key(a):
                dA[a] += 1
            else:
                dA.setdefault(a,1)
        for b in lB:
            if not dA.has_key(b):
                return False
            
            else:
                dA[b] -= 1
                if dA[b]<0:
                    return False           
        return True
        
            
def main():
    sol = Same()
    #"This is nowcoder","is This nowcoder"
    if sol.checkSam("Here you are","Are you here"):
        
        print 'true'
    else:
        print 'false'
if __name__ == '__main__':
    main()