import { Component, OnInit } from '@angular/core';
import { LifecycleService } from './lifecycle.service';
import { ServiceState } from './lifecycle.model';

@Component({
  selector: 'app-lifecycle',
  templateUrl: './lifecycle.component.html',
  styleUrls: ['./lifecycle.component.css']
})
export class LifecycleComponent implements OnInit {

  constructor(private lifecycleService: LifecycleService) { }

  protected services: ServiceState[] = [
    {
      serviceName: 'PGCS',
      time: '',
      serviceState: 'waiting'
    },
    {
      serviceName: 'NRNCS',
      time: '',
      serviceState: 'waiting'
    },
    {
      serviceName: 'ContentApp1',
      time: '',
      serviceState: 'waiting'
    },
    {
      serviceName: 'ContentApp2',
      time: '',
      serviceState: 'waiting'
    }
  ];

  ngOnInit(): void {
    setInterval(() => {
      this.setServicesLifeCycle();
    }, 2000);
  }

  setServicesLifeCycle() {
    this.lifecycleService.getOfferedServices().subscribe(data => {
      if (data.serviceStates.length > 0)
      {
        this.services = data.serviceStates;
      }
    });
  }

}
