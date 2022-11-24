import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

import { catchError, retry } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable()
export class DataMonitorService {

    constructor(private http: HttpClient) { }

    SERVER_ADRESS = '/api/';
    SERVICE_OFFERED = 'serviceOffers';
    TRANSFERED_DATA = 'publishedMessages';

    getOfferedServices(): Observable<any> {
        return this.http.get(`${this.SERVER_ADRESS}${this.SERVICE_OFFERED}`).pipe(
            retry(3),
            catchError(this.handleError)
        );
            
    }

    getTransferedData(): Observable<any> {
        return this.http.get(`${this.SERVER_ADRESS}${this.TRANSFERED_DATA}`).pipe(
            retry(3),
            catchError(this.handleError)
        );
            
    }

    private handleError(error: any) {
        const errMsg = (error.message) ? error.message :
            error.status ? `${error.status} - ${error.statusText}` : 'Server error';
        return errMsg;
    }
}