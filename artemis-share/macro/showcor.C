void showcor(UShort_t val, UShort_t val2, UShort_t val3)
{

   printf("%x %x %x\n",val,val2,val3);
   printf("val2 | val3              = %x\n",(val2|val3));
   printf("(val2 + val3)>>1         = %x\n",(val2+val3)>>1);
   printf("~((val2 + val3)>>1)      = %x\n",0xfff&(~((val2+val3)>>1)));
   printf("abs(val2 - val3)         = %x\n",abs(val2-val3));
   printf("~abs(val2 - val3)        = %x\n",0xfff&(~abs(val2-val3)));
   printf("val2 & val3              = %x\n",(val2&val3));
   printf("val&~(val2|val3)         = %x\n",(val&~(val2|val3)));
   printf("val&~(val2&val3)         = %x\n",(val&~(val2&val3)));
   printf("val - (val&~(val2|val3)) = %x\n",val-(val&~(val2|val3)));
   printf("val - ((0xfff&~abs(val2-val3))&(val&~(val2|val3)) = %x\n",val-((0xfff&(~abs(val2-val3)>>1))&(val&~((val2+val3)>>1))));
}
