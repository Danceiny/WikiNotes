约定：sum = Segma(Xi), sum2 = Segma(Xi^2),....

```Java
// 五個基本量
public class MeasureCalculate{
    public long count;
    public double sum;
    public double sum2;
    public double sum3;
    public double sum4;

    public MeasureCalculator(){
        count = 0;
        sum = 0.0;
        sum2 = 0.0;
        sum3 = 0.0;
        sum4 = 0.0;
        //count = 0;
    }

    public void calculate(double d){
        sum += d;
        sum2 += d * d;
        sum3 += d * d * d;
        sum4 += d * d * d * d;
        count ++;
    }

    public void calculate(MeasureCalculator mc){
        this.sum += mc.sum;
        this.sum2 += mc.sum2;
        this.sum3 += mc.sum3;
        this.sum4 += mc.sum4;
        this.count += mc.count;
    }
}

    mean = sum / count;
    if(1 == count){
        variance = 0.0;
    }
    else{
        variance = (sum2 - sum * mean) / (count - 1);
    }
    standardDeviation = Math.sqrt(variance);
    cv = standardDeviation / mean;  // coefficient of variation
    standError = standardDeviation / Math.sqrt(count);

    moment2 = sum2 / count;
    moment3 = sum3 / count;
    moment4 = sum4 / count;

    centralMoment2 = (sum2 - mean * sum) / count;
    centralMoment3 = (sum3 - mean * sum2 * 3 + 2 * sum * mean * mean) / count;
    centralMoment4 = (sum4 - 4 * sum3 * mean + 6 * sum2 * mean * mean - 3 * sum * mean * mean * mean) / count;

    skewness = centralMoment3 / (centralMoment2 * Math.sqrt(centralMoment2));

    kurtosis = centralMoment4 / (centralMoment2 * centralMoment2) - 3;

```    
