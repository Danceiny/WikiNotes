import re
input_str_for_test = ['{{[(2+3)*(1-3)]+4}*(14-3)}',
                        '{{[(2+3)*(1-3)]+4}*(14-3))}']
re_pattern = '[{}()\[\]]'
priority_map = {'{':3,'[':2,'(':1,')':-1,']':-2,'}':-3}
def is_valide_repr(input_str):
    tokens = re.findall(re_pattern, input_str)
    stack = []
    priority = 0
    for token in tokens:
        priority = priority_map.get('token')
        if token not in priority_map.keys():
            continue
        if token == '{':
            if '[' in stack or '(' in stack:
                return False
            stack.append(token)
        elif token == '[':
            if '[' in stack or '(' in stack:
                return False
            stack.append(token)
        elif token == '(':
            if '(' in stack:
                return False
            stack.append(token)
        elif token == '}':
            if len(stack) and stack[-1] == '{':
                stack.pop()
            else:
                return False
        elif token == ']':
            if len(stack) and stack[-1] == '[':
                stack.pop()
            else:
                return False
        elif token == ')':
            if len(stack) and stack[-1] == '(':
                stack.pop()
            else:
                return False
    if priority > 1:
        return False
    return True

if __name__ == '__main__':
    for s in input_str_for_test:
        print is_valide_repr(s)