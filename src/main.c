#include<stdio.h>
#include<time.h>
#include<string.h>
int main()
{
    float voltage,current,pf,power,hours,energy,rate,bill;
    float maxpower=500,totalEnergy=0.0,totalBill=0.0,maxEnergy=0.0,minEnergy=0.0,highEnergyPercentage=0.0,moderateEnergyPercentage=0.0,lowEnergyPercentage=0.0;
    int isTodayReport=0,reportCount=0,highEnergyCount=0,lowEnergyCount=0,moderateEnergyCount=0;
    time_t currentTime;
    struct tm*localTime;
    char dateTime[50];
    char todayDate[20];
    char savedDate[20];
    char line[200];
    FILE *fp;
    printf(" Enter voltage: ");
    scanf("%f", &voltage);
    printf(" Enter current: ");
    scanf("%f", &current);
    printf(" Enter power factor(0 to 1): ");
    scanf("%f", &pf);
    printf(" Enter time in hours : ");
    scanf("%f",&hours);
    printf(" Enter rate per unit: ");
    scanf("%f", &rate);
    if(voltage<=0)
    {
    	printf(" Invalid voltage!\n");
	    return 1;
    }
    if(current<=0)
    {
    	printf(" Invalid current!\n");
	    return 1;
    }
    if(pf<=0 || pf>1)
    {
    	printf(" Invalid power factor!\n");
	    return 1;
    }
    if(hours<=0)
    {
    	printf(" Invalid time in hours!\n");
	    return 1;
    }
    if(rate<=0)
    {
    	printf(" Invalid rate!\n");
	    return 1;
    }
    power = voltage * current * pf;
    energy = (power*hours)/1000;
    bill = energy*rate;
    if(power>maxpower)
        printf(" Warning: Abnormal power consumption!\n");
    else
    printf(" Power consumption is normal.\n");
    if(energy<1)
        printf("Low consumption\n");
    else if(energy>=1 && energy<=2)
        printf(" Moderate consumption\n");
    else
        printf(" High consumption\n");
    currentTime=time(NULL);
    localTime=localtime(&currentTime);
    if(localTime==NULL)
    {
        printf(" Unable to get localtime!");
        return 1;
    }
    strftime(dateTime, sizeof(dateTime),"%d-%m-%y %H:%M:%S",localTime);
    strftime(todayDate,sizeof(todayDate),"%d-%m-%y",localTime);
    printf(" Today's date:[%s]\n",todayDate);
    strcpy(todayDate,dateTime);
    todayDate[8]='\0';
    printf(" Date and Time: %s\n", dateTime);
    printf("\n ===== ENERGY REPORT =====\n");
    printf(" Power = %.2f W\n", power);
    printf(" Energy = %.2f kwh\n",energy);
    printf(" Bill in rupees = %.2f rupees\n",bill);
    fp=fopen("energy_report.txt","a");
    if(fp==NULL)
    {
        printf(" Error opening file!\n");
        return 1;
    }
    fprintf(fp, "\n ===== ENERGY REPORT =====\n");
    fprintf(fp, " Date and Time: %19s\n", dateTime);
    fprintf(fp, " Power = %.2f W\n",power);
    fprintf(fp, " Energy = %.2f kwh\n",energy);
    fprintf(fp, " Bill in rupees = %.2f rupees\n",bill);
    fclose(fp);
    fp=fopen("energy_report.txt","r");
    if(fp==NULL)
    {
        printf(" Unable to open file for reading!\n");
        return 1;
    }
    printf("\n ===== SAVED REPORTS =====\n");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, "Date and Time:") != NULL)
        {
            sscanf(line, "Date and Time: %19s", savedDate);
            if(strcmp(savedDate,todayDate)==0)
            {
                isTodayReport=1;
                reportCount++;
            }
            else
            {
                isTodayReport=0;
            }
        }
        if (isTodayReport && strstr(line, "Energy = ") != NULL)
        {
            sscanf(strstr(line, "=") + 1, "%f", &energy);
            totalEnergy += energy;
            if(energy>2.0)
            {
                highEnergyCount++;
            }
            if(energy<1.0)
            {
                lowEnergyCount++;
            }
            if(energy>=1.0 && energy <=2.0)
            {
                moderateEnergyCount++;
            }
            if(energy>maxEnergy)
            {
                maxEnergy=energy;
            }
            if(energy<minEnergy || minEnergy==0.0)
            {
                minEnergy=energy;
            }
        }
        if (isTodayReport && strstr(line, "Bill in rupees = ") != NULL)
        {
            sscanf(strstr(line, "=") + 1, "%f", &bill);
            totalBill += bill;
        }
    }
    if (reportCount > 0)
    {
        highEnergyPercentage =((float)highEnergyCount / reportCount) * 100;
        moderateEnergyPercentage =((float)moderateEnergyCount / reportCount) * 100;
        lowEnergyPercentage =((float)lowEnergyCount / reportCount) * 100;
    }
    printf("\n Total Energy Consumption = %.2f kwh\n", totalEnergy);
    printf("\n Highest Energy Consumption Today = %.2f kwh\n", maxEnergy);
    printf("\n Lowest Energy Consumption Today = %.2f kwh\n", minEnergy);
    printf("\n Total Reports Today = %d\n", reportCount);
    if (reportCount > 0)
    {
        printf(" Average Energy Consumption Today = %.2f kwh\n",
        totalEnergy / reportCount);
    }
    printf("\n ========== ENERGY CLASSIFICATION SUMMARY ==========\n");
    printf("\nCategory             Reports     Percentage\n");
    printf("--------------------------------------------\n");
    printf("%-20s %-12d %.2f%%\n","High Energy", highEnergyCount, highEnergyPercentage);
    printf("%-20s %-12d %.2f%%\n","Moderate Energy", moderateEnergyCount, moderateEnergyPercentage);
    printf("%-20s %-12d %.2f%%\n","Low Energy", lowEnergyCount, lowEnergyPercentage);
    printf("--------------------------------------------\n");
    printf("%-20s %-12d 100.00%%\n", "Total", reportCount);
    printf("============================================\n");
    printf("\n Total Electricity Bill = Rs. %.2f\n", totalBill);
    fclose(fp);
    return 0;
}