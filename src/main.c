#include <stdio.h>
#include <time.h>
int main()
{
    float voltage,current,pf,power,hours,energy,rate,bill;
    float maxpower=500,totalEnergy=0;
    time_t currentTime;
    struct tm*localTime;
    char dateTime[50];
    char line[200];
    FILE *fp;
    printf("Enter voltage: ");
    scanf("%f", &voltage);
    printf("Enter current: ");
    scanf("%f", &current);
    printf("Enter power factor(0 to 1): ");
    scanf("%f", &pf);
    printf("Enter time in hours : ");
    scanf("%f",&hours);
    printf("Enter rate per unit: ");
    scanf("%f", &rate);
    if(voltage<=0)
    {
    	printf("Invalid voltage!\n");
	    return 1;
    }
    if(current<=0)
    {
    	printf("Invalid current!\n");
	    return 1;
    }
    if(pf<0 || pf>1)
    {
    	printf("Invalid power factor!\n");
	    return 1;
    }
    if(hours<=0)
    {
    	printf("Invalid time in hours!\n");
	    return 1;
    }
    if(rate<=0)
    {
    	printf("Invalid rate!\n");
	    return 1;
    }
    power = voltage * current * pf;
    energy = (power*hours)/1000;
    bill = energy*rate;
    if(power>maxpower)
        printf("Warning: Abnormal power consumption!\n");
    else
    printf("Power consumptionis normal.\n");
    if(energy<1)
        printf("Low consumption\n");
    else if(energy>=1 && energy<=2)
        printf("Moderate consumption\n");
    else
        printf("High consumption\n");
    currentTime=time(NULL);
    localTime=localtime(&currentTime);
    if(localTime==NULL)
    {
        printf("Unable to get localtime!");
        return 1;
    }
    strftime(dateTime, sizeof(dateTime),"%d-%m-%y %H:%M:%S",localTime);
    printf("Date and Time: %s\n", dateTime);
    printf("\n===== ENERGY REPORT =====\n");
    printf("Power = %.2f W\n", power);
    printf("Energy = %.2f kwh\n",energy);
    printf("Bill in rupees = %.2f rupees\n",bill);
    fp=fopen("energy_report.txt","a");
    if(fp==NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(fp, "\n===== ENERGY REPORT =====\n");
    fprintf(fp, "Date and Time: %s\n", dateTime);
    fprintf(fp, "Power = %.2f W\n",power);
    fprintf(fp, "Energy = %.2f kwh\n",energy);
    fprintf(fp, "Bill in rupees = %.2f rupees\n",bill);
    fclose(fp);
    fp=fopen("energy_report.txt","r");
    if(fp==NULL)
    {
        printf("Unable to open file for reading!\n");
        return 1;
    }
    printf("\n===== SAVED REPORTS =====\n");
    while(fgets(line,sizeof(line),fp)!=NULL)
    {
        printf("%s",line);
    }
    fclose(fp);
    return 0;
}