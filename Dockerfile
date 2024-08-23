FROM ubuntu:24.10
COPY build/chestnut/chestnut /chestnut
ENTRYPOINT [ "/chestnut" ]
COPY static /etc/chestnut/static

ENV HTTP_STATIC_DIR=/etc/chestnut/static
ENV METRICS_PORT=8000
ENV HTTP_PORT=80
ENV ENABLE_WEATHER=1
ENV WEATHER_BASE_URL=api.openweathermap.org
ENV WEATHER_LAT="40.6526006"
ENV WEATHER_LON="-73.9497211"
ENV WEATHER_TIMEZONE="-5"
EXPOSE 80
