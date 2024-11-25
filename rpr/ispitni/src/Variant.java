public class Variant {
    int a,b,c,d;
    public Variant(int a, int b, int c, int d) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
    }
    public int sracunaj() {
        if (this.a < this.b && this.c > this.d) return this.a + this.c;
        if (this.a > this.b && this.c == this.d && this.c == 0) return this.a + this.b;
        if (this.a > this.b && this.c <= this.d) return this.a + this.d;
        if (this.a < this.b) return this.c + this.d;
        return 0;
    }
}
