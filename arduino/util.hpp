
template <typename... Args>
void serialPrintf(const char *s, Args... args)
{
    static char buffer[255];
    sprintf(buffer, s, args...);
    Serial.print(buffer);
}