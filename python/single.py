def single(num,step=0):
    #This function can check the multiplicity of a single number of arbitrary size.
    #There is technically no need to use c++ for this, since it's computationally not very hard to
    #multiply numbers together.
    print(step,"--->",num)
    digits = [int(i) for i in str(num)]
    next = 1
    for i  in digits:
        next*=i
    if(next<10):
        step+=1
        print(step, "--->", next)
        print("DONE")
        print("Total steps: ",step)
        return
    step+=1
    single(next,step)